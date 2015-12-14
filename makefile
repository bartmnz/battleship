CFLAGS= -std=c11 -Wall -Werror -Wextra -pedantic -Wno-deprecated  -Wstack-usage=1024 

TARGET1=battleShip

OBJS1=battleShip.o 

.PHONY: clean debug profile

$(TARGET1): $(OBJS1)



debug: CFLAGS+=-g
debug: all


clean:
	-$(RM) $(TARGET1) $(OBJS1) 

profile: CFLAGS+=-pg
profile: LDFLAGS+=-pg
profile: $(TARGET)