#include "aduc812.h"
#include "lcd.h"
#include "max.h"

void main( void ) {
    init_lcd();
    print_string_lcd("Hello!", 6);
}
