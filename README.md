# Projeto-Interpretador-Para-Microcontrolador
Intepretador básico escrito em C++ para microcontroladores com pelo menos 128KB de flash que possibilita que o microcontrolador seja capaz de interpretar e rodar programas criados pelos a partir de arquivos TXT em um cartão microSD, sem que a memória FLASH tenha de ser reprogramada.
(testado em um Arduino Uno R4)

Permite:
- Gravação de variáveis na memória EEPROM interna do microcontrolador (Cada variável ocupa 12 Bytes: os 4 primeiros Bytes são o nome da variável e os outros 8 Bytes são o valor da variável)
- Realização de contas usando combinação de (VAR+VAR), (NumeroConstante+NumeroConstante), (VAR+NumeroConstante), (NumeroConstante+VAR), e atribuição do resultado dessas contas para outras variáveis
- Ativação dos pinos GPIO do Arduino
- Leitura da memória EEPROM, usando vários formatos diferentes
- Apagar variáveis da memória
- Suporte a ler e executar programas a partir de cartões SD

Comandos:
- EEPROM_ERASE()       : Apaga todo o EEPROM interno. SINTAXE: EEPROM_ERASE() 
- EEPROM_INFO()        : Mostra informações sobre o EEPROM. SINTAXE: EEPROM_INFO()
- EEPROM_WR_FLOAT()    : Grava variável na memória usando o endereço desejado. SINTAXE: EEPROM_WRITE_FLOAT(VARIABLE NAME WITH 4 CHARS, ADDRESS: FROM 0 TO (EEPROM LENGTH/12), VALUE)
- EEPROM_READ_FROM()   : Realiza leitura dos Bytes do eeprom dentro do range selecionado. SINTAXE: EEPROM_READ_FROM(ADDRESS START,ADDRESS END, ADDRESS CONTENT FORMATTING[BIN, HEX, DEC])
- FUNC()               : Realiza contas de divisão, multiplicação, soma, subtração entre 2 numeros. SINTAXE: FUNC(N1/VAR1, N2/VAR2)
- WR_FLOAT()           : Grava variável na memória sem ter que indicar um endereço. SINTAXE: WR_FLOAT XXXX = YYYYYYY... 
- DIGITAL_WRITE()      : Muda o estado do pino selecionado. SINTAXE: DIGITAL_WRITE(PINNUMBER, HIGH/LOW)
