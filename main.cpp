#include <cstdio>               // puts
#include <cstdlib>              // EXIT_FAILURE
#include <iostream>             // cout, cerr, endl
#include <string>               // string
#include <getopt.h>             // getopt_long
#include "distro_fetcher.hpp"   // DistroFetcher

using std::string;
using std::cout;
using std::cerr;
using std::endl;

void ShowHelp(void) noexcept
{
    std::puts("Usage: ubuntu_fetcher [options]\n"
              "Options:\n"
              "  --help, -h                Show this help message\n"
              "  --list-releases           List all supported Ubuntu releases\n"
              "  --current-lts             Get the current Ubuntu LTS version\n"
              "  --hash <release date>     Get the SHA256 hash digest of the 'disk1.img' file for a specific release\n"
              "\n"
              "Examples:\n"
              "    ubuntu_fetcher --list-releases\n"
              "    ubuntu_fetcher --hash 2025-04-03\n");
}

int main(int const argc, char **const argv)
{
    static constexpr struct option long_options[] = {
        {"help",          no_argument,       nullptr, 'h'},
        {"list-releases", no_argument,       nullptr,  0 },
        {"current-lts",   no_argument,       nullptr,  1 },
        {"hash",          required_argument, nullptr,  2 },
        {nullptr, 0, nullptr, 0 }
    };

    DistroFetcher fetcher("https://cloud-images.ubuntu.com/releases/streams/v1/com.ubuntu.cloud:released:download.json");

    int option_index = 0, c = 0;
    while ( -1 != (c = getopt_long(argc, argv, "h", long_options, &option_index)) )
    {
        switch ( c )
        {
        case 'h':
            ShowHelp();
            return 0;

        case 0:  // --list-releases
            {
                cout << "Date           Version   Codename\n"
                        "------------------------------------------\n";
                auto const releases = fetcher.GetSupportedReleases();
                for ( auto const &r : releases ) cout << r << '\n';
                return 0;
            }

        case 1:  // --current-lts
            {
                cout << "Current Ubuntu LTS version: " << fetcher.GetCurrentLTSVersion() << endl;
                return 0;
            }

        case 2:  // --hash
            if ( nullptr != optarg )
            {
                string const digest = fetcher.GetDisk1Sha256(optarg);
                cout << "Hash SHA256 digest of file 'disk1.img' for release date " << optarg << ": " << digest << endl;
                return 0;
            }
            else
            {
                cerr << "Error: Please provide a release name for --hash option.\n";
                return EXIT_FAILURE;
            }

        default:
            ShowHelp();
            return EXIT_FAILURE;
        }
    }

    ShowHelp();
    return EXIT_FAILURE;
}
