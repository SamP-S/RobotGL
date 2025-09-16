#pragma once

namespace marathon {
    
class App {
private:
    bool _mQuit = false;
    bool _mFailed = false;

protected:
    App();
    ~App();

    void Boot();
    void Shutdown();

public:
    // main methods for user to use
    void Run();
    void Quit();

    // virtual methods for user to implement
    virtual void Start() = 0;
    virtual void Update(double deltaTime) = 0;
    
};

} // namespace marathon
