//them mot tin nhan moiw

// 74HC165 
sbit DU_LIEU_165 = P3^7;   // Chan DATA doc tu 74HC165
sbit CLOCK_165   = P3^6;   // Chan CLOCK dich bit
sbit LOAD_165    = P3^5;   // Chan nap du lieu song song

// Nut RESET 
sbit NUT_RESET = P3^4;     // Nut reset he thong

// LED 
#define LED P1             // Port dieu khien LED


unsigned char tam;          // Bien tam luu du lieu

unsigned char du_lieu_30;   // Byte gui LCD
unsigned char du_lieu_31;
unsigned char du_lieu_32;
unsigned char du_lieu_33;


// KHAI BAO HAM
void delay_ms(unsigned int t);

void I2C_START();
void I2C_STOP();
void GUI_DIA_CHI();
void NHAN_ACK();
void GUI_DU_LIEU(unsigned char value);

void KHOI_TAO_LCD_4BIT();
void CAI_DAT_LCD();

void GUI_LENH_LCD(unsigned char value);
void GUI_KY_TU_LCD(unsigned char value);

void TACH_DU_LIEU(unsigned char value);
void TACH_DU_LIEU_KY_TU(unsigned char value);

void lcd_string(char *s);

unsigned char doc_74HC165();


// HAM DELAY
void delay_ms(unsigned int t)
{
    unsigned int i, j;

    // Tao tre bang vong lap rong
    for(i = 0; i < t; i++)
        for(j = 0; j < 120; j++);
}

// HAM BAT DAU I2C
void I2C_START()
{
    SDA = 1;      // SDA muc cao
    SCL = 1;      // SCL muc cao

    SDA = 0;      // SDA xuong thap -> tao START
}

// HAM KET THUC I2C
void I2C_STOP()
{
    SCL = 0;      // Clock muc thap
    SDA = 0;      // SDA muc thap

    SCL = 1;      // Clock muc cao
    SDA = 1;      // SDA muc cao -> tao STOP
}

// HAM GUI DIA CHI LCD I2C
void GUI_DIA_CHI()
{
    unsigned char i;

    // Dia chi LCD I2C PCF8574
    unsigned char value = 0x4E;

    for(i = 0; i < 8; i++)
    {
        SCL = 0;                  // Tat xung clock

        SDA = (value & 0x80);     // Gui tung bit MSB

        value <<= 1;              // Dich trai bit tiep theo

        SCL = 1;                  // Tao xung clock
    }
}

// HAM NHAN TIN HIEU ACK

void NHAN_ACK()
{
    SCL = 0;      // Tat clock

    SDA = 1;      // Nha SDA de slave keo xuong

    SCL = 1;      // Tao xung clock

    while(SDA);   // Doi ACK tu slave
}


//  GUI 1 BYTE DU LIEU I2C
void GUI_DU_LIEU(unsigned char value)
{
    unsigned char i;

    for(i = 0; i < 8; i++)
    {
        SCL = 0;                  // Tat clock

        SDA = (value & 0x80);     // Gui bit cao nhat

        value <<= 1;              // Dich trai

        SCL = 1;                  // Tao xung clock
    }

    NHAN_ACK();                   // Nhan ACK
}

// KHOI TAO LCD 4 BIT
void KHOI_TAO_LCD_4BIT()
{
    // Tach lenh 0x28 thanh 4bit cao va 4 bit thap
    TACH_DU_LIEU(0x28);

    // Gui 4bit cao
    GUI_DU_LIEU(du_lieu_30);

    // Gui 4bit thap
    GUI_DU_LIEU(du_lieu_31);
}


//  HAM CAI DAT LCD
void CAI_DAT_LCD()
{
    GUI_LENH_LCD(0x28);   // LCD 4 bit, 2 dong

    GUI_LENH_LCD(0x0C);   // Bat hien thi, tat con tro

    GUI_LENH_LCD(0x06);   // Tu dong tang vi tri con tro

    GUI_LENH_LCD(0x01);   // Xoa man hinh

    delay_ms(5);
}

//  HAM GUI LENH LCD
void GUI_LENH_LCD(unsigned char value)
{
    // Tach byte thanh 4bit cao va 4bit thap
    TACH_DU_LIEU(value);

    // Gui 4bit cao
    GUI_DU_LIEU(du_lieu_30);
    GUI_DU_LIEU(du_lieu_31);

    // Gui 4bit thap
    GUI_DU_LIEU(du_lieu_32);
    GUI_DU_LIEU(du_lieu_33);

    delay_ms(2);
}

//  GUI KY TU RA LCD
void GUI_KY_TU_LCD(unsigned char value)
{
    // Tach byte ky tu thanh 4bit cao va 4 bit thap
    TACH_DU_LIEU_KY_TU(value);

    // Gui 4bit cao
    GUI_DU_LIEU(du_lieu_30);
    GUI_DU_LIEU(du_lieu_31);

    // Gui 4bit thap
    GUI_DU_LIEU(du_lieu_32);
    GUI_DU_LIEU(du_lieu_33);

    delay_ms(2);
}


// GUI CHUOI KY TU LCD
void lcd_string(char *s)
{
    while(*s)
    {
        GUI_KY_TU_LCD(*s++);   // Gui tung ky tu
    }
}

//          TACH BYTE DU LIEU KY TU LCD
void TACH_DU_LIEU_KY_TU(unsigned char value)
{
    tam = value;

    //4bit cao + EN = 1
    value &= 0xF0;

    value |= 0x0D;

    du_lieu_30 = value;

    //4bit cao + EN = 0 
    value = tam;

    value &= 0xF0;

    value |= 0x09;

    du_lieu_31 = value;

    //4bit thap + EN = 1
    value = tam;

    value &= 0x0F;

    value |= 0xD0;

    value = (value << 4) | (value >> 4);

    du_lieu_32 = value;

    //4bit thap + EN = 0 
    value = tam;

    value &= 0x0F;

    value |= 0x90;

    value = (value << 4) | (value >> 4);

    du_lieu_33 = value;
}

// TACH BYTE LENH LCD
void TACH_DU_LIEU(unsigned char value)
{
    tam = value;

    // 4bit cao + EN = 1 
    value &= 0xF0;

    value |= 0x0C;

    du_lieu_30 = value;

    //4bit cao + EN = 0 
    value = tam;

    value &= 0xF0;

    value |= 0x08;

    du_lieu_31 = value;

    //4bit thap + EN = 1 
    value = tam;

    value &= 0x0F;

    value |= 0xC0;

    value = (value << 4) | (value >> 4);

    du_lieu_32 = value;

    //4bit thap + EN = 0
    value = tam;

    value &= 0x0F;

    value |= 0x80;

    value = (value << 4) | (value >> 4);

    du_lieu_33 = value;
}

// DOC DU LIEU 74HC165
unsigned char doc_74HC165()
{
    unsigned char i;

    unsigned char value = 0;

    // Nap du lieu song song vao thanh ghi
    LOAD_165 = 0;

    delay_ms(1);

    LOAD_165 = 1;

    // Doc tung bit noi tiep
    for(i = 0; i < 8; i++)
    {
        value <<= 1;      // Dich trai tao cho trong

        // Neu DATA = 1 thi them bit 1
        if(DU_LIEU_165)
            value |= 1;

        // Tao xung clock dich bit
        CLOCK_165 = 1;

        delay_ms(1);

        CLOCK_165 = 0;
    }

    return value;
}

//                      HAM MAIN
void main()
{
    unsigned char du_lieu_tho;     // Du lieu doc tu 74HC165

    unsigned char du_lieu_nut;     // Du lieu sau debounce

    unsigned char so_nut_bat;      // Dem so nut dang bat

    unsigned char i;

    //              KHOI TAO LCD

    I2C_START();           // Bat dau giao tiep I2C

    GUI_DIA_CHI();         // Gui dia chi LCD

    NHAN_ACK();            // Nhan ACK

    KHOI_TAO_LCD_4BIT();   // Khoi tao LCD 4 bit

    CAI_DAT_LCD();         // Cai dat LCD

    //                  VONG LAP CHINH
    while(1)
    {
        //                  KIEM TRA RESET
        if(NUT_RESET == 0)
        {
            delay_ms(20);   // Chong doi phim

            if(NUT_RESET == 0)
            {
                LED = 0x00;   // Tat toan bo LED

                // Xoa LCD
                GUI_LENH_LCD(0x01);

                // Dong 1
                GUI_LENH_LCD(0x80);

                lcd_string("00000000");

                // Dong 2
                GUI_LENH_LCD(0xC0);

                lcd_string("So nut: 0");

                // Doi nha nut
                while(NUT_RESET == 0);
            }
        }

        //              DOC DU LIEU 74HC165
        du_lieu_tho = doc_74HC165();

        //                  CHONG DOI PHIM
        delay_ms(10);

        // Neu du lieu thay doi thi bo qua
        if(du_lieu_tho != doc_74HC165())
            continue;

        // Luu du lieu nut nhan
        du_lieu_nut = du_lieu_tho;

        //                  XUAT LED
        LED = du_lieu_nut;

        //              DEM SO NUT BAT
        so_nut_bat = 0;

        for(i = 0; i < 8; i++)
        {
            // Neu bit = 1 thi tang bien dem
            if(du_lieu_nut & (1 << i))
                so_nut_bat++;
        }

        //              HIEN THI LCD DONG 1
        GUI_LENH_LCD(0x80);

        for(i = 0; i < 8; i++)
        {
            // Hien thi trang thai tung nut
            if(du_lieu_nut & (1 << (7 - i)))
                GUI_KY_TU_LCD('1');
            else
                GUI_KY_TU_LCD('0');
        }

        //              HIEN THI LCD DONG 2
        GUI_LENH_LCD(0xC0);

        lcd_string("SO NUT BAT: ");

        // Hien thi so nut dang bat
        GUI_KY_TU_LCD(so_nut_bat + '0');

        GUI_KY_TU_LCD(' ');

        delay_ms(100);
    }
}
