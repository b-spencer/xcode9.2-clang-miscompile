CXX := xcrun clang++
LD := $(CXX)

SYSROOT := $(shell xcodebuild -version -sdk macosx Path |head -1)
ifeq ($(SYSROOT),,)
  $(error "Can't find macosx sysroot")
endif

COMMONFLAGS := \
  -arch x86_64 \
  -isysroot $(SYSROOT)

# Passing -fsanitize=undefined or -fsanitize=address finds no errors, but
# _avoids_ the bug!
COMMONFLAGS += $(if $(SANITIZE),-fsanitize=$(SANITIZE))

CXXFLAGS := \
  $(COMMONFLAGS) \
  -std=c++11 -stdlib=libc++ \
  -Wall -Werror

# Removing -g avoids the crash.
CXXFLAGS += $(if $(NO_DEBUG),,-g)

# Using this makes it not crash.
#CXXFLAGS += -gline-tables-only

# Optimization levels that crash (with -g): -Os -Oz -O3 -O2 -Ofast
# Optimization levels that work (with -g) : -O1 -O0
CXXFLAGS += -Os


LDFLAGS := $(COMMONFLAGS)

all:
	$(CXX) --version
	$(CXX) -c $(CXXFLAGS) Thing.cc -o Thing.o
	$(CXX) -c $(CXXFLAGS) main.cc -o main.o
	$(LD) $(LDFLAGS) -o prog Thing.o main.o

clean:
	rm *.o prog
