NAME		:= client
LIBS		:= m GL glfw GLEW freetype

CC 			:= gcc
CFLAGS 		:= -std=c99 -march=native -pipe -Wall -Wextra -Wshadow -I/usr/include/freetype2
CXX 		:= g++
CXXFLAGS 	:= -std=c++11 -march=native -pipe -Wall -Wextra -Wshadow -I/usr/include/freetype2
LD 			:= g++
LDFLAGS 	:=
AS			:= as
ASFLAGS		:= 


# debug on
DEBUG_ON_CFLAGS 		:= -O0 -ggdb -DDEBUG
DEBUG_ON_CXXFLAGS 		:= -O0 -ggdb -DDEBUG
DEBUG_ON_LDFLAGS 		:= -O0 -ggdb -DDEBUG
DEBUG_ON_ASFLAGS 		:=

# debug off
DEBUG_OFF_CFLAGS 		:= -O2
DEBUG_OFF_CXXFLAGS 		:= -O2
DEBUG_OFF_LDFLAGS 		:= -O2
DEBUG_OFF_ASFLAGS 		:=

# profile on
PROFILE_ON_CFLAGS 		:= -pg
PROFILE_ON_CXXFLAGS 	:= -pg
PROFILE_ON_LDFLAGS 		:= -pg
PROFILE_ON_ASFLAGS 		:=

# profile off
PROFILE_OFF_CFLAGS 		:=
PROFILE_OFF_CXXFLAGS 	:=
PROFILE_OFF_LDFLAGS 	:=
PROFILE_OFF_ASFLAGS 	:=
