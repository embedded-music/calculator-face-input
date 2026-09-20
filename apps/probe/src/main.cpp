#include <Arduino.h>
#include <M5Unified.h>
#include <Wire.h>

namespace {
constexpr uint8_t CALCULATOR_I2C_ADDRESS = 0x08;
constexpr uint8_t CALCULATOR_INTERRUPT_PIN = 5;
constexpr uint32_t I2C_FREQUENCY_HZ = 100000;

uint32_t eventCount = 0;

bool calculatorAcknowledges() {
  Wire.beginTransmission(CALCULATOR_I2C_ADDRESS);
  return Wire.endTransmission() == 0;
}

bool readCalculatorByte(uint8_t& value) {
  const uint8_t received =
      Wire.requestFrom(CALCULATOR_I2C_ADDRESS, uint8_t{1});
  if (received != 1 || !Wire.available()) {
    return false;
  }

  value = Wire.read();
  return true;
}

char printableCharacter(uint8_t value) {
  return value >= 0x20 && value <= 0x7E ? static_cast<char>(value) : '.';
}

void reportCalculatorEvent(uint8_t value) {
  eventCount++;
  Serial.printf(
      "calculator: event=%lu value=0x%02X decimal=%u char=%c irq=%u ms=%lu\n",
      static_cast<unsigned long>(eventCount),
      value,
      value,
      printableCharacter(value),
      digitalRead(CALCULATOR_INTERRUPT_PIN),
      static_cast<unsigned long>(millis()));

  M5.Display.fillRect(0, 70, M5.Display.width(), 90, TFT_BLACK);
  M5.Display.setCursor(0, 70);
  M5.Display.setTextSize(1);
  M5.Display.printf("Event: %lu\n", static_cast<unsigned long>(eventCount));
  M5.Display.printf("Hex: 0x%02X\n", value);
  M5.Display.printf("Decimal: %u\n", value);
  M5.Display.printf("Character: %c\n", printableCharacter(value));
}

void reportCoreButton(const char* name, const char* action) {
  Serial.printf("core_button: name=%s action=%s ms=%lu\n",
                name,
                action,
                static_cast<unsigned long>(millis()));

  M5.Display.fillRect(0, 170, M5.Display.width(), 30, TFT_BLACK);
  M5.Display.setCursor(0, 170);
  M5.Display.setTextSize(1);
  M5.Display.printf("core_%s: %s\n", name, action);
}

void showProbeStatus(bool found) {
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Display.setTextSize(2);
  M5.Display.println("Calculator Face");
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.setTextSize(1);
  M5.Display.println();
  M5.Display.printf("I2C: 0x%02X\n", CALCULATOR_I2C_ADDRESS);
  M5.Display.printf("IRQ: GPIO %u\n", CALCULATOR_INTERRUPT_PIN);
  M5.Display.println(found ? "Calculator detected" : "Calculator NOT found");
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);

  auto config = M5.config();
  config.internal_spk = false;
  config.internal_mic = false;
  M5.begin(config);

  pinMode(CALCULATOR_INTERRUPT_PIN, INPUT_PULLUP);
  Wire.begin(21, 22, I2C_FREQUENCY_HZ);

  const bool found = calculatorAcknowledges();
  showProbeStatus(found);
  Serial.printf(
      "calculator_face: address=0x%02X irq_pin=%u detected=%s\n",
      CALCULATOR_I2C_ADDRESS,
      CALCULATOR_INTERRUPT_PIN,
      found ? "yes" : "no");
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) reportCoreButton("a", "pressed");
  if (M5.BtnA.wasReleased()) reportCoreButton("a", "released");
  if (M5.BtnB.wasPressed()) reportCoreButton("b", "pressed");
  if (M5.BtnB.wasReleased()) reportCoreButton("b", "released");
  if (M5.BtnC.wasPressed()) reportCoreButton("c", "pressed");
  if (M5.BtnC.wasReleased()) reportCoreButton("c", "released");

  if (digitalRead(CALCULATOR_INTERRUPT_PIN) == LOW) {
    uint8_t value = 0;
    if (readCalculatorByte(value)) {
      reportCalculatorEvent(value);
    } else {
      Serial.println("calculator: read_error");
    }
  }

  delay(1);
}

