//  vim:ts=2:sw=2:et
//==============================================================================
/// @brief  Create a fanout device file
/// @author Serge Aleynikov <saleyn@gmail.com>
/// @date   2016-08-18
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

#include<stdio.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int get_device_id(char* devname);

int main(int argc, char* argv[]) {
  mode_t mode;
  dev_t  dev;
  int    error;
  int    verbose = argc > 1 && strcmp(argv[1], "-v")==0;
  int    i       = verbose ? 2 : 1;
  int    devid   = get_device_id("fanout");

  if (devid < 0) {
	perror("fanout device not found");
    exit(1);
  }

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-v] NewFanoutDeviceFileName\n", argv[0]);
    exit(1);
  }

  if (verbose)
    fprintf(stderr, "fanout-dev-id: %d\n", devid);

  char* path = argv[i];
  dev        = makedev(devid, 0);
  mode       = S_IFCHR | 0664;
  error      = mknod(path, mode, dev);
  if (error < 0) {
    perror("mknod");
    return -1;
  }

  if (verbose) 
    fprintf(stderr, "filename.....: %s\n", path);

  return 0;
}

int get_device_id(char* devname) {
  FILE* f     = fopen("/proc/devices", "r");
  int   devid = -1;
  char  line[256];
  int   line_num = 1;

  if (!f)
	goto DONE;

  // Will hold each line scanned in from the file
  while (!feof(f)) {
    char* s = fgets(line, sizeof(line), f);

    if (!s)
	  goto DONE;

	int  num;
	char dev[256];
	int  n = sscanf(s, "%d %s\n", &num, dev);

	if (n != 2) continue;

    if (strcmp(dev, devname) == 0) {
	  devid = num;
	  goto DONE;
    }
  }

DONE:
  // Close file and free line
  fclose(f);
  return devid;
}
