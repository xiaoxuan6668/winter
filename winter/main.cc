#include "winter/application.h"

int main(int argc, char** argv) {
    winter::Application app;
    if(app.init(argc, argv)) {
        return app.run();
    }
    return 0;
}