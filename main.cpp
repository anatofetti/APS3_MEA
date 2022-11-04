#include "mbed.h"

AnalogIn Vel(PB_1);         // Potênciometro Velocidade
DigitalIn EN(USER_BUTTON);  // Botão Enable

class MB { //Classe do motor brushless, criada para evitar fazer os mesmos
            //comandos várias vezes e não poluir tanto o código
    private:
        PwmOut Xp; //Reconhecimento das entradas das bobinas do motor
        DigitalOut EN;//Reconhecimento da variável de acionamento de cada bobina
        
    public:
        // Métodos
        void Control(float vel, int ena) {
            Xp.write(vel); //A velocidade, controlada pelo potenciômetro
            EN.write(ena); //O booleano de acionamento de cada bobina, se está
                            //ligada ou não de acordo com a ordem dos passos
        }
    
        // Inicializador
        MB(PinName Fase, PinName enFase): Xp(Fase), EN(enFase)
    //Ao iniciar, são reconhecidos os pinos de cada bobina utilizados no projeto
        { 
            Xp.period_ms(1); //Período para o PWM do motor
            Xp.write(0); // Váriáveis resetadas
            EN.write(0);
        }
        
        ~MB() {
        }
};

int main() {
    // Aqui entra efetivamente o reconhecimento das entradas do motor
    MB MB_A(PA_8, PC_10); 
    MB MB_B(PA_9, PC_11);
    MB MB_Z(PA_10, PC_12);
    int wait = 3; //Tempo entre acionmento das fases em ms
    
    while(1) {
        if(EN == 0) { //Quando o botão start/stop do usuário é acionado
            float vel = Vel.read(); //Leitura do potenciômetro, definindo a
                                    //velocidade de saída do motor
                                    
            //Aqui há o acionamento sucessivo de fases, acionando duas entradas
            //ao mesmo tempo a fim de criar os 6 passos requisitados
            
            // passo 0
            MB_A.Control(vel, 1);
            MB_B.Control(0 ,  1);
            MB_Z.Control(0,   0);
            wait_ms(wait);
            
            // passo 1
            MB_A.Control(vel, 0);
            MB_B.Control(0,   1);
            MB_Z.Control(vel, 1);
            wait_ms(wait);
            
            // passo 2
            MB_A.Control(0,   1);
            MB_B.Control(0,   0);
            MB_Z.Control(vel, 1);
            wait_ms(wait);
            
            // passo 3
            MB_A.Control(0,   1);
            MB_B.Control(vel, 1);
            MB_Z.Control(0,   0);
            wait_ms(wait);
            
            // passo 4
            MB_A.Control(0,   0);
            MB_B.Control(vel, 1);
            MB_Z.Control(0,   1);
            wait_ms(wait);
            
            // passo 5
            MB_A.Control(vel, 1);
            MB_B.Control(0,   0);
            MB_Z.Control(0,   1);
            wait_ms(wait);
        }
        else { // motor desligado caso o usuário deixe de apertar o botão
            MB_A.Control(0, 0);
            MB_B.Control(0, 0);
            MB_Z.Control(0, 0);
        }
    }
}












