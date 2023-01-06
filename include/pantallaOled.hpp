#define ALTO 64
#define ANCHO 128
Adafruit_SSD1306 display(ANCHO, ALTO, &Wire, -1);

bool iniciaDisplay()
{
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    return display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

static void dibujaPrimeraLinea()
{
    int ancho = ANCHO-8;
    display.drawLine(ancho, 15, ancho, map(1, 1, 5, 10, 1), SSD1306_WHITE);
}
static void dibujaSegundaLinea()
{
    int ancho = ANCHO-6;
    display.drawLine(ancho, 15, ancho, map(2, 1, 5, 10, 1), SSD1306_WHITE);
}
static void dibujaTerceraLinea()
{
    int ancho = ANCHO-4;
    display.drawLine(ancho, 15, ancho, map(3, 1, 5, 10, 1), SSD1306_WHITE);
}
static void dibujaCuartaLinea()
{
    int ancho = ANCHO-2;
    display.drawLine(ancho, 15, ancho, map(4, 1, 5, 10, 1), SSD1306_WHITE);
}
static void dibujaQuintaLinea()
{
    int ancho = ANCHO;
    display.drawLine(ancho, 15, ancho, map(5, 1, 5, 10, 1), SSD1306_WHITE);
}
void dibujaEstadoWifi(int senial,int8_t RSSI)
{
    display.setCursor(ANCHO - 20,0);
    display.print(RSSI);
    if (senial >= 1)
    {
        dibujaPrimeraLinea();
    }
    if (senial >= 2)
    {
        dibujaSegundaLinea();
    }
    if (senial >= 3)
    {
        dibujaTerceraLinea();
    }
    if (senial >= 4)
    {
        dibujaCuartaLinea();
    }
    if (senial >= 5)
    {
        dibujaQuintaLinea();
    }
}
