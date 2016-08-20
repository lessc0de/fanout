//  vim:ts=2:sw=2:et
//==============================================================================
/// @brief     Test fanout reader
//------------------------------------------------------------------------------
/// @author    Serge Aleynikov <saleyn@gmail.com>
/// @date      2016-08-18
//  @copyright Copyright (c) 2016, Serge Aleynikov
//------------------------------------------------------------------------------
//  Fanout device copyright (c) 2010-2015, Bob Smith
//==============================================================================
/*
Copyright (c) 2016, Serge Aleynikov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>

int64_t now() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (int64_t)ts.tv_sec*1000000000l + ts.tv_nsec;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s FanoutDevFile [Count]\n", argv[0]);
    exit(1);
  }

  int count  = (argc > 2 ? atoi(argv[2]) : 1000000) - 1;
  int fd = open(argv[1], O_WRONLY);

  if (fd < 0) {
    perror("open");
    exit(1);
  }

  long start  = now();
  uint8_t tm;

  for (long i=0; i < count; ++i) {
    tm = i % 254 + 1;
    if (write(fd, &tm, sizeof(tm)) < 0) {
      perror("write");
      exit(1);
    }
  }

  tm = 0;
  if (write(fd, &tm, sizeof(tm)) < 0) {
    perror("write");
    exit(1);
  }

  count++;

  long end = now();
DONE:
  close(fd);

  long duration = end - start;
  printf("Count  : %ld\n",   count);
  printf("Speed  : %.0f/s\n", 1000000000.0*count / duration);
  printf("Latency: %.3fus\n", (double)duration / 1000000.0 / count);

  return 0;
} 
