#include "Printer.h"

//----------------------------------------------------------------------------------------


// konstruktor, laduje czcionke przy pomocy freetype
Printer::Printer(const char* filepath)
{
    VAO = 0;
    VBO = 0;
    FT_Library ft; // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR: FREETYPE: Blad trzy inicjacji bibloteki" << std::endl;
    }
    else
    {
        std::cout << "INFO: FREETYPE: Zaladowano pomyslnie" << std::endl;
    }

    FT_Face face; // load font as face
    if (FT_New_Face(ft, filepath, 0, &face))
    {
        std::cout << "ERROR: FREETYPE: Blad przy wczytywaniu czcionki = " << filepath << std::endl;
    }
    else
    {
        std::cout << "INFO: FREETYPE: Pomyslnie wczytano czcionke = " << filepath << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 48); // set size to load glyphs, 0 daje liczenie dynamicznie
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    for (unsigned char c = 0; c < 128; c++) // load first 128 characters of ASCII set
    { 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) // Load character glyph 
        {
            std::cout << "ERROR: FREETYPE: Blad ladowania znaku" << std::endl;
            continue;
        }
        unsigned int texture; // generate texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face); // destroy FreeType once we're finished
    FT_Done_FreeType(ft);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}//----------------------------------------------------------------------------------------


// renderowanie tekstu @string text
void Printer::draw(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    shader.use(); // activate corresponding render state	
    glUniform3f(glGetUniformLocation(shader.get_id(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    std::string::const_iterator c; // iterate through all characters
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        float vertices[6][4] = { // update VBO for each character
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        }; 
        glBindTexture(GL_TEXTURE_2D, ch.TextureID); // render glyph texture over quad
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // update content of VBO memory
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6); // render quad
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6)* scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}//----------------------------------------------------------------------------------------


// wlaczenie opcji opengl do poprawnego wyswietlania tekstu
void Printer::prepare()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}//----------------------------------------------------------------------------------------


// przywrocenie opcji domyslnych
void Printer::restore()
{
    glBlendFunc(GL_ONE, GL_ZERO);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

void Printer::config(Shader& shader)
{
    shader.use();
    shader.setMat4("projection", glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT)));
}