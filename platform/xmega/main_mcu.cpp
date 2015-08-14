#include <avr/interrupt.h>
#include "cpp.hh"
#include "components.hh"
#include "setup.hh"

#define STOP_IF_ERROR(x) {if(x){while(1){}}}

void PlaySound(BuzzerSound sound)
{
    GetComponents()->GetBuzzer()->SetSound(sound);
}

void ButtonStateChangedCallback()
{
    Buttons *buttons = GetComponents()->GetButtons();
    Buzzer *buzzer = GetComponents()->GetBuzzer();
    UiMain *uimain = GetComponents()->GetUiMain();

    // Update buttons
    if (buttons->GetDown() == Buttons::BUTTON_LONG || buttons->GetDown() == Buttons::BUTTON_SHORT) {
        uimain->KeyPress(UiBase::KEY_DOWN, true);
        buzzer->Beep();
    }
    if (buttons->GetUp() == Buttons::BUTTON_LONG || buttons->GetUp() == Buttons::BUTTON_SHORT) {
        uimain->KeyPress(UiBase::KEY_UP, true);
        buzzer->Beep();
    }
    if (buttons->GetCenter() == Buttons::BUTTON_SHORT) {
        uimain->KeyPress(UiBase::KEY_RIGHT, true);
        buzzer->Beep();
    } else if (buttons->GetCenter() == Buttons::BUTTON_LONG) {
        uimain->KeyPress(UiBase::KEY_LEFT, true);
        buzzer->Beep();
    }
    if (buttons->GetCenter() == Buttons::BUTTON_OFF &&
        buttons->GetUp() == Buttons::BUTTON_EXTRA_LONG &&
        buttons->GetDown() == Buttons::BUTTON_EXTRA_LONG)
    {
        // Zero altitude
        GetComponents()->GetSensorController()->ZeroAltitude();
        buzzer->Beep(3);
    }
}

void UpdateFromConfig(Config *conf)
{  
    GetComponents()->GetDisplayBuffer()->SetRotation(conf->display_orientation);
    GetComponents()->GetUiMain()->GetAltimeterUi()->SetUiMode(conf->default_altimeter_ui_mode);
}

void UpdateFromConfigAfterSave(Config *conf)
{
    if (conf->log_save_mode == Config::DataSaveAll) {
        GetComponents()->GetSensorController()->StartSavingData();
    } else if (conf->log_save_mode == Config::DataSaveOff) {
        GetComponents()->GetSensorController()->StopSavingData();
    }
}

void SaveConfig(Config *conf)
{
    UpdateFromConfigAfterSave(conf);
    GetComponents()->GetMemoryController()->WriteConfig(conf);
}

uint8_t LoadConfig(Config *conf)
{
    return GetComponents()->GetMemoryController()->LoadConfig(conf);
}

int main()
{
    Components components;
    SetComponents(&components);

    components.GetUiMain()->SetConfigChangedFunction(UpdateFromConfig);
    components.GetUiMain()->SetConfigSaveFunction(SaveConfig);
    components.GetDisplay()->SetDisplayOn(true);
    components.GetClock()->Setup();
    components.GetClock()->Setup1HzSquareWave();
    components.GetDigipot()->Setup();
    components.GetDigipot()->SetValue(255);
    components.GetSensorController()->Setup();
    components.GetSensorController()->SetMiscInformation(components.GetMiscInformation());
    components.GetDisplay()->Setup();
    components.GetDisplay()->Clear();
    components.GetSensors()->SetMiscInformation(components.GetMiscInformation());
    components.GetAltitudeManager()->SetPlaySoundFunction(PlaySound);
    components.GetButtons()->SetButtonStateChangedFunction(ButtonStateChangedCallback);

    SetupHardware();

    LoadConfig(components.GetConfig());
    UpdateFromConfig(components.GetConfig());
    UpdateFromConfigAfterSave(components.GetConfig());

    while (1) {

    }
}
