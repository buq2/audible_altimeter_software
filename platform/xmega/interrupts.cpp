#include "interrupts.hh"
#include "components.hh"
#include "usb.hh"

bool global_init_function_run_after_reset = false;
void InitFunctionAfterReset()
{
    static uint8_t times_run = 0;
    ++times_run;
    if (times_run == 3) {
        // Altitude sensors are not valid right after reset.
        // Zero the altitude few times before satisfied...
        global_init_function_run_after_reset = true;
    }
    GetComponents()->GetSensorController()->ZeroAltitude();
}

void UpdateDisplayAndUi()
{
    // Display related
    DisplaySharp *display = GetComponents()->GetDisplay();
    UiMain *uimain = GetComponents()->GetUiMain();
    display->ToggleExtcomin();
    uimain->Tick100ms();

    DisplayBuffer *buffer = GetComponents()->GetDisplayBuffer();
    uimain->Render(buffer);
    display->SetContent(*buffer);
}

void UpdateSensors(float update_seconds)
{
    SensorController *sensorctrl = GetComponents()->GetSensorController();

    sensorctrl->DataUpdate(update_seconds);
    // Update altimeters
    sensorctrl->RequestDataUpdate();
}

// buzzer timer
ISR(TCE0_OVF_vect)
{
    GetComponents()->GetBuzzer()->Tick10ms();
}

// 1HZ interrupt from the clock
ISR(PORTB_INT0_vect)
{
    if (!global_init_function_run_after_reset) {
        // Init function is run ~1s after reset
        // Before this the altitude module does not have valid altitude
        InitFunctionAfterReset();
    }
}

// Button press interrupt
ISR(PORTD_INT0_vect)
{
    GetComponents()->GetButtons()->CheckState();
}

// 100ms interrupt
ISR(RTC_OVF_vect)
{
    static uint8_t update_200ms = 1;
    update_200ms = !update_200ms;
    if (update_200ms) {
        // Update sensors once 200ms
        UpdateSensors(0.2);
        GetComponents()->GetAltitudeManager()->Tick200ms();
    }
    UpdateDisplayAndUi();
    UpdateUsb();
}

ISR(TCD0_OVF_vect)
{
    Buttons *buttons = GetComponents()->GetButtons();
    if (0 != buttons) {
        buttons->UpdateButtonCounters();
    }
}
