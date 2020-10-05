#include "Initialization.h"
//----------------------------------------------------------------------------------------


// domyslny konstruktor
Initialization::Initialization(const int width, const int height)
{
    this->height_ = height;
    this->width_ = width;
    this->window_ = nullptr;
}//----------------------------------------------------------------------------------------


// inicjacja glfw, stworzenie okna programu
void Initialization::initialize_glfw(const char* title)
{
    glfwInit(); // po tej opcji mozemy konfigurowac glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // mowimy ktora opcje konfigurujemy
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // deklaracja wersji opengl 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window_ = glfwCreateWindow(width_, height_, title, nullptr, nullptr); // obiekt okna, potrzebny do innych funkcji
    if (window_ == nullptr)
    {
        std::cout << "ERROR: GLFW: Blad przy tworzeniu okna" << std::endl;
        glfwTerminate();
    }
    else
    {
        std::cout << "INFO: GLFW: Okno utworzono pomyslnie" << std::endl;
        glfwMakeContextCurrent(window_);
    }
}//-----------------------------------------------------------------------------------------


// inicjacja biblioteki glew
void Initialization::initialize_glad()
{
    char msg[256];
    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
    {
        std::cout << "ERROR: GLAD: Nie udalo sie zainicjowac" << std::endl;
    }
    else
    {
        std::cout << "INFO: GLAD: Zaladowanie przebieglo pomyslnie" << std::endl;
    }
    sprintf(msg, "");
    strcat(msg, reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    std::cout << "INFO: OPENGL: Dostawca uslugi = " << msg << std::endl;

    sprintf(msg, "");
    strcat(msg, reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    std::cout << "INFO: OPENGL: Karta graficzna = " << msg << std::endl;

    sprintf(msg, "");
    strcat(msg, reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    std::cout << "INFO: OPENGL: Wersja sterownika = " << msg << std::endl;
}//----------------------------------------------------------------------------------------


// ustawia callbacki
void Initialization::set_callbacks()
{
    glfwSetFramebufferSizeCallback(window_, &Callback::frame_buffer_size_callback);
    glfwSetCursorPosCallback(window_, &Callback::mouse_callback);
    glfwSetScrollCallback(window_, &Callback::scroll_callback);
    glfwSetErrorCallback(&Callback::error_callback);
}//----------------------------------------------------------------------------------------


// window getter
GLFWwindow *Initialization::get_window()
{
    return window_;
}//----------------------------------------------------------------------------------------


// destruktor
Initialization::~Initialization()
{
    std::cout << "INFO: " << "===================== Wylaczono program =======================" << std::endl << std::endl<< std::endl;
    glfwTerminate();
}//----------------------------------------------------------------------------------------





