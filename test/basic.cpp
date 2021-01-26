#include <photonGui/window.hpp>
#include <thread>

int main(void)
{
    photonGui::window test;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}