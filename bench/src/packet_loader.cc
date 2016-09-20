#include "packet_loader.h"

const char* usage =
    "Usage: %s -n [numthreads] -r [ratelimit] -t [maxtime] [data] [attrs]\n";

void print_usage(char *exec) {
  fprintf(stderr, usage, exec);
}

int main(int argc, char** argv) {
  if (argc < 3 || argc > 9) {
    print_usage(argv[0]);
    return -1;
  }

  int c;
  int num_threads = 1;
  uint8_t num_attributes = 1;
  uint64_t timebound = UINT64_MAX;
  uint64_t rate_limit = 0;
  while ((c = getopt(argc, argv, "t:n:h:r:")) != -1) {
    switch (c) {
      case 't':
        timebound = atol(optarg) * 1000000;
        break;
      case 'n':
        num_threads = atoi(optarg);
        break;
      case 'r':
        rate_limit = atoll(optarg);
        break;
      default:
        LOG(stderr, "Could not parse command line arguments.\n");
    }
  }

  if (optind == argc) {
    print_usage(argv[0]);
    return -1;
  }

  std::string data_path = std::string(argv[optind]);
  std::string attr_path = std::string(argv[optind + 1]);

  if (rate_limit == 0) {
    packet_loader<> loader(data_path, attr_path);
    loader.load_packets(num_threads, timebound, 0);
  } else {
    packet_loader<rate_limiter> loader(data_path, attr_path);
    loader.load_packets(num_threads, timebound, rate_limit);
  }

  return 0;
}
