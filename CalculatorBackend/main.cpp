#include <cstdlib>

#include "CalcResourceFactory.h"
#include "CalcServiceSettingsFactory.h"
#include "CalcService.h"

int main(int argc, const char* argv[])
{
    if (argc != 5) {
        std::cout << "USAGE:\n"
                  << argv[0]
                  << " tcp://192.168.2.146:1883 clientId username password\n";
        return 1; 
    }
    const char *ADDRESS_ = argv[1];
    const char *CLIENTID_ = argv[2];
    const char *USERNAME_ = argv[3];
    const char *PASSWORD_ = argv[4];

    auto calc_resource_factory = make_shared<CalcResourceFactory>(ADDRESS_, CLIENTID_, USERNAME_, PASSWORD_);
    auto calc_service_settings_factory =
        make_shared<CalcServiceSettingsFactory>();

    CalcService calc_service{
        calc_resource_factory, calc_service_settings_factory };

    std::cout << "\n*** start ***\n";
    calc_service.start();
    std::cout << "\n*** bye ***\n";

    return EXIT_SUCCESS;
}
