#include <photon/window.hpp>
#include <thread>

int main(void)
{
    photon::window test;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}