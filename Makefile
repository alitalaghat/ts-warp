# ------------------------------------------------------------------------------
# TS-Warp - Transparent SOCKS protocol Wrapper
# ------------------------------------------------------------------------------

# Copyright (c) 2021, 2022, Mikhail Zakharov <zmey20000@yahoo.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# ------------------------------------------------------------------------------
CC = cc
CFLAGS += -Wall
WARP_OBJS = inifile.o natlook.o pidfile.o socks.o ts-warp.o utils.o xedec.o
PASS_OBJS = ts-pass.o xedec.o 

all:	ts-warp ts-pass

ts-warp: $(WARP_OBJS)
	$(CC) -o $@ $(WARP_OBJS)

ts-pass: $(PASS_OBJS)
	$(CC) -o $@ $(PASS_OBJS)

clean:
	rm -rf ts-warp ts-pass *.o *.dSYM *.core

inifile.o: inifile.h
natlook.o: natlook.h
pidfile.o: pidfile.h
socks.o: socks.h
ts-warp.o: ts-warp.h
utils.o: utils.h
