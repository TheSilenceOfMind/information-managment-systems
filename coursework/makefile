# ---------------------------------------------------------------------------
# Имя проекта

NAME	= multicooker

# Настройки компилятора и линкера

CC      = sdcc
CFLAGS  = -I./include -c --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80 

# Настройки системы автоинкремента версии сборки

PROJECT  = MULTICOOKER_SDK_1_1
VERSION  = 1.0.0
BUILD    = 0
TYPE     = w

PROJNAME = ${PROJECT}-${VERSION}-${BUILD}-${TYPE}
TARBALL  = ${PROJNAME}.tar

# Настройки M3P

M3P		 = m3p
COMPORT	 = com4
COMLOG	 = $(COMPORT)_log.txt
BAUD	 = 9600	

# Каталоги с исходными текстами

SRC_DIR = src
# ---------------------------------------------------------------------------

all: multicooker

clean:
	del $(NAME).hex
	del $(NAME).bin
	del $(NAME).map
	del $(NAME).mem
	del $(NAME).lnk
	del pm3p_*.txt
	del com?_log.txt
	del $(TARBALL).gz
	del $(SRC_DIR)\*.asm
	del $(SRC_DIR)\*.rel
	del $(SRC_DIR)\*.rst
	del $(SRC_DIR)\*.sym
	del $(SRC_DIR)\*.lst 

load:
	$(M3P) lfile load.m3p


dist:
	tar -cvf $(TARBALL) --exclude=*.tar .
	gzip $(TARBALL)

term:
	$(M3P) echo $(COMLOG) $(BAUD)  openchannel $(COMPORT) +echo 6 term -echo bye



MULTICOOKER_SRC = \
$(SRC_DIR)/keyboard.c \
$(SRC_DIR)/lcd.c \
$(SRC_DIR)/led.c \
$(SRC_DIR)/max.c \
$(SRC_DIR)/multicooker.c 

MULTICOOKER_OBJ = $(MULTICOOKER_SRC:.c=.rel)

multicooker : $(MULTICOOKER_OBJ) makefile
	$(CC) $(MULTICOOKER_OBJ) -o multicooker.hex $(LFLAGS)
	$(M3P) hb166 multicooker.hex multicooker.bin bye


$(MULTICOOKER_OBJ) : %.rel : %.c makefile
	$(CC) -c $(CFLAGS) $< -o $@
