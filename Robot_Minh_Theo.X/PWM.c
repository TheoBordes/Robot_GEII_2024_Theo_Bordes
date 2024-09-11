#include <xc.h>
#include "IO.h"
#include "PWM.h"
#include "Toolbox.h"
#include "robot.h"

#define PWMPER 24.0

float acceleration = 1.0;
float talon = 20;

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //�Priode en pourcentage
    //�Rglage PWM moteur 1 sur hacheur 1
    IOCON1bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON1bits.PENL = 1;
    IOCON1bits.PENH = 1;
    FCLCON1 = 0x0003; //�Dsactive la gestion des faults
    IOCON2bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON2bits.PENL = 1;
    IOCON2bits.PENH = 1;
    FCLCON2 = 0x0003; //�Dsactive la gestion des faults
    /* Enable PWM Module */
    PTCONbits.PTEN = 1;
}

//void PWMSetSpeed(float vitesseEnPourcents, int moteur) { premier test des moteurs
//    if (Abs(vitesseEnPourcents) > 100) {
//        return;
//    }
//    switch (moteur) {
//        case MOTEUR_GAUCHE:
//            if (vitesseEnPourcents < 0) {
//                SDC1 = Abs(vitesseEnPourcents) * PWMPER + talon;
//                PDC1 = talon;
//            } else {
//                PDC1 = vitesseEnPourcents * PWMPER + talon;
//                SDC1 = talon;
//            }
//            LED_ROUGE_2 = 1;
//            break;
//        case MOTEUR_DROIT:
//            if (vitesseEnPourcents < 0) {
//                PDC2 = Abs(vitesseEnPourcents) * PWMPER + talon;
//                SDC2 = talon;
//            } else {
//                SDC2 = vitesseEnPourcents * PWMPER + talon;
//                PDC2 = talon;
//            }
//            LED_ROUGE_1 = 1;
//            break;
//    }
//}

void PWMUpdateSpeed() {
    // Cette fonction est appelee sur timer et permet de suivre des rampes d acceleration
    if (robotState.vitesseDroiteCommandeCourante < robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Min(robotState.vitesseDroiteCommandeCourante + acceleration, robotState.vitesseDroiteConsigne);

    if (robotState.vitesseDroiteCommandeCourante > robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Max(robotState.vitesseDroiteCommandeCourante - acceleration, robotState.vitesseDroiteConsigne);

    if (robotState.vitesseDroiteCommandeCourante >= 0) {
        PDC1 = robotState.vitesseDroiteCommandeCourante * PWMPER + talon;
        SDC1 = talon;
    } else {
        PDC1 = talon;
        SDC1 = -robotState.vitesseDroiteCommandeCourante * PWMPER + talon;
    }

    if (robotState.vitesseGaucheCommandeCourante < robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Min(robotState.vitesseGaucheCommandeCourante + acceleration, robotState.vitesseGaucheConsigne);

    if (robotState.vitesseGaucheCommandeCourante > robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Max(robotState.vitesseGaucheCommandeCourante - acceleration, robotState.vitesseGaucheConsigne);

    if (robotState.vitesseGaucheCommandeCourante > 0) {
        PDC2 = robotState.vitesseGaucheCommandeCourante * PWMPER + talon;
        SDC2 = talon;
    } else {
        PDC2 = talon;
        SDC2 = -robotState.vitesseGaucheCommandeCourante * PWMPER + talon;
    }
}

PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur) {
     if (Abs(vitesseEnPourcents) > 100) {
        return;
    }
    switch (moteur) {
        case MOTEUR_GAUCHE:
                robotState.vitesseGaucheConsigne = vitesseEnPourcents;
            break;
        case MOTEUR_DROIT:
                 robotState.vitesseDroiteConsigne = -vitesseEnPourcents ;
            break;
    }
}