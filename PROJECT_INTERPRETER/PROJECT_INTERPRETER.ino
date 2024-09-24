#include <EEPROM.h>
String version = "0";
String code;
bool REPL = true;
int EEPROM_CODE_START_ADDRESS = 512;
int EEPROM_SIZE = EEPROM.length();  

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.print("\nArduTerpter By E.B. Version ");
  Serial.print(version);
  Serial.print("\nEEPROM: ");
  Serial.print(EEPROM.length());
  Serial.print(" BYTES");
  Serial.print(" | AVAILABLE VARIABLE EEPROM SPACE: ");
  int AVAILABLE_VARIABLE_EEPROM_SPACE = 0;
  
  for(int i = 0; i < EEPROM_CODE_START_ADDRESS/12; i++)
  {
    int AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = 0;

    for(int j = 0; j < 12; j++)
    {
      if(EEPROM.read((i*12) + j) == 0)
      {
        AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = AVAILABLE_BYTES_AT_VARIABLE_ADDRESS + 1;
      }
    }

    if(AVAILABLE_BYTES_AT_VARIABLE_ADDRESS == 12)
    {
      AVAILABLE_VARIABLE_EEPROM_SPACE = AVAILABLE_VARIABLE_EEPROM_SPACE + 1;
    }
  }

  Serial.print(AVAILABLE_VARIABLE_EEPROM_SPACE * 12);
  Serial.print(" BYTES");
  Serial.print(" | AVAILABLE VARIABLE EEPROM SPACE: ");
  Serial.print(EEPROM.length());
  Serial.print("\n");
}

//THE FUNCTION BELOW IS RESPONSIBLE FOR DECIDING IF A STRING IS A VARIABLE OR A CONSTANT NUMBER IN THE COMMANDS THAT NEED THIS DISTINCTION.
String VALUE_ANALYZER(String str)
{
    int number_amount = 0;
    for(int i = 0; i < str.length(); i++)
    {

      if(str.charAt(i) == '(' || str.charAt(i) == ')' || str.charAt(i) == ',' || str.charAt(i) == ' ' || str.charAt(i) == '=')
      {
        return "error";
      }else if(str.charAt(i) != '1' && str.charAt(i) != '2' && str.charAt(i) != '3' && str.charAt(i) != '4' && str.charAt(i) != '5' && str.charAt(i) != '6' && 
               str.charAt(i) != '7' && str.charAt(i) != '8' && str.charAt(i) != '9' && str.charAt(i) != '0' && str.charAt(i) != '.' && str.length() == 4)
      {
        return "VAR";
      }else if(str.charAt(i) == '1' || str.charAt(i) == '2' || str.charAt(i) == '3' || str.charAt(i) == '4' || str.charAt(i) == '5' || str.charAt(i) == '6' || 
               str.charAt(i) == '7' || str.charAt(i) == '8' || str.charAt(i) == '9' || str.charAt(i) == '0' || str.charAt(i) == '.')
      {

        number_amount = number_amount + 1;
      }else
      {
        return "error";
      }

      if(number_amount == str.length())
      {
        return "CONST_NUM";
      }
    }
}

double FUNC()
{
  //SYNTAX: FUNC(N1/VAR1, N2/VAR2)
  String funcStart = "FUNC(";
  
  if (code.startsWith(funcStart)) 
  {
    int whilecounter = 5;
    String n1;
    String n1_mode;

    String n2;
    String n2_mode;
    char op = ' ';
    bool error = false;

    while(code.charAt(whilecounter) != '+' && code.charAt(whilecounter) != '-' && code.charAt(whilecounter) != '/' && code.charAt(whilecounter) != '*')
    {
      n1.concat(code.charAt(whilecounter));
      whilecounter++;
    }
    op = code.charAt(whilecounter);

    //ANALYZES THE NUMBER 1 STRING TO DETERMINE IF IT IS A VARIABLE OR A CONSTANT NUMBER
    n1_mode = VALUE_ANALYZER(n1);

    if(n1_mode == "VAR")
    {
      bool varFound = false;
      int varAddress;

      //LOOP CHECKS FOR EXISTING VARIABLES WITH THE SAME NAME IN EEPROM
      for(int i = 0; i <= (EEPROM_CODE_START_ADDRESS/12)-1; i++)
      {
        String varNameDuplicate;

        if(varFound != false)
        {
          break;
        }

        for(int j = 0; j < 4; j++)
        {
          char varNameFromMemory = EEPROM.read((i*12) + j);

          if(varNameFromMemory == n1.charAt(j))
          {
            varNameDuplicate.concat(varNameFromMemory);
          }

          if(n1 == varNameDuplicate)
          {
            varFound = true;
            varAddress = i;
            break;
          }
        }
      }

      if(varFound == true)
      {
        double varPrint;
        EEPROM.get((varAddress * 12)+4, varPrint);
        n1 = varPrint;
      }else
      {
        error = true;
        Serial.print("\n>: ERROR: VARIABLE NOT FOUND ");
        return NULL;
      }
    }else if(n1_mode == "CONST_NUM")
    {
      //NO ACTION NEEDED, N1 ALREADY CONTAINS THE DESIRED CONST_NUM.
    }else if(n1_mode == "error")
    {
      error = true;
      return NULL;
    }
    
    whilecounter++;
    if(error == false && code.charAt(code.length() - 2) == ')')
    {
      while(code.charAt(whilecounter) != ')')
      {
        n2.concat(code.charAt(whilecounter));
        whilecounter++;
      }

      //ANALYZES THE NUMBER 2 STRING TO DETERMINE IF IT IS A VARIABLE OR A CONSTANT NUMBER
      n2_mode = VALUE_ANALYZER(n2);

      if(n2_mode == "VAR")
      {
        bool varFound = false;
        int varAddress;

        //LOOP CHECKS FOR EXISTING VARIABLES WITH THE SAME NAME IN EEPROM
        for(int i = 0; i <= (EEPROM_CODE_START_ADDRESS/12)-1; i++)
        {
          String varNameDuplicate;

          if(varFound != false)
          {
            break;
          }

          for(int j = 0; j < 4; j++)
          {
            char varNameFromMemory = EEPROM.read((i*12) + j);

            if(varNameFromMemory == n2.charAt(j))
            {
              varNameDuplicate.concat(varNameFromMemory);
            }

            if(n2 == varNameDuplicate)
            {
              varFound = true;
              varAddress = i;
              break;
            }
          }
        }

        if(varFound == true)
        {
          double varPrint;
          EEPROM.get((varAddress * 12)+4, varPrint);
          n2 = varPrint;
        }else
        {
          error = true;
          Serial.print("\n>: ERROR: VARIABLE NOT FOUND ");
          return NULL;
        }
      }else if(n2_mode == "CONST_NUM")
      {
        //NO ACTION NEEDED, N1 ALREADY CONTAINS THE DESIRED CONST_NUM.
      }else if(n2_mode == "error")
      {
        error = true;
        return NULL;
      }

      if(error == false && code.charAt(whilecounter) == ')')
      {
        if(op == '+')
        {
          if(REPL)
          {
            Serial.print("\n>: ");
            Serial.print(n1.toDouble() + n2.toDouble());
          }

          return n1.toDouble() + n2.toDouble();
        }else if(op == '-')
        {
          if(REPL)
          {
            Serial.print("\n>: ");
            Serial.print(n1.toDouble() - n2.toDouble());
          }

          return n1.toDouble() - n2.toDouble();
        }if(op == '/')
        {
          if(REPL)
          {
            Serial.print("\n>: ");
            Serial.print(n1.toDouble() / n2.toDouble());
          }

          return n1.toDouble() / n2.toDouble();
        }if(op == '*')
        {
          if(REPL)
          {
            Serial.print("\n>: ");
            Serial.print(n1.toDouble() * n2.toDouble());
          }

          return n1.toDouble() * n2.toDouble();
        }
        
      }else
      {
        Serial.print("\n>: SYNTAX ERROR1 ");
        return NULL;
      }

    }else
    {
      Serial.print("\n>: SYNTAX ERROR2 ");
      return NULL;
    }
  }
}

void EEPROM_ERASE()
{
  String EEerase = "EEPROM_ERASE()";
  
  if(code.startsWith(EEerase))
  {
    Serial.print("\n\n");
    for (int i = 0 ; i < EEPROM.length() ; i++) 
    {
      if(i % 64 == 0)
      {
        Serial.print(" ");
        Serial.print(".");
      }
      EEPROM.write(i, 0);
    }

    Serial.print("\n>: EEPROM ERASED SUCCESSFULLY");
  }
}

void EEPROM_READ_FROM()
{
  //SYNTAX: EEPROM_READ_FROM(ADDRESS START,ADDRESS END, ADDRESS CONTENT FORMATTING[BIN, HEX, DEC])

  String EEreadStart = "EEPROM_READ_FROM(";
  String EEreadEnd = ")";
  
  if(code.startsWith(EEreadStart))
  {
    int whilecounter = 17;
    String address1;
    String address2;
    String op;

    if(code.charAt(whilecounter) == '1' || code.charAt(whilecounter) == '2' || code.charAt(whilecounter) == '3' || code.charAt(whilecounter) == '4' || code.charAt(whilecounter) == '5' ||
       code.charAt(whilecounter) == '6' || code.charAt(whilecounter) == '7' || code.charAt(whilecounter) == '8' || code.charAt(whilecounter) == '9' || code.charAt(whilecounter) == '0')
    {
      while(code.charAt(whilecounter) == '1' || code.charAt(whilecounter) == '2' || code.charAt(whilecounter) == '3' || code.charAt(whilecounter) == '4' || code.charAt(whilecounter) == '5' ||
            code.charAt(whilecounter) == '6' || code.charAt(whilecounter) == '7' || code.charAt(whilecounter) == '8' || code.charAt(whilecounter) == '9' || code.charAt(whilecounter) == '0' || code.charAt(whilecounter) == '.')
      {
        address1.concat(code.charAt(whilecounter));
        whilecounter++;
      }

      if(code.charAt(whilecounter) == ',')
      {
        whilecounter++;

        while(code.charAt(whilecounter) == '1' || code.charAt(whilecounter) == '2' || code.charAt(whilecounter) == '3' || code.charAt(whilecounter) == '4' || code.charAt(whilecounter) == '5' ||
            code.charAt(whilecounter) == '6' || code.charAt(whilecounter) == '7' || code.charAt(whilecounter) == '8' || code.charAt(whilecounter) == '9' || code.charAt(whilecounter) == '0' || code.charAt(whilecounter) == '.')
        {
          address2.concat(code.charAt(whilecounter));
          whilecounter++;
        }

        if(code.charAt(whilecounter) == ',')
        {
          whilecounter++;

          if(code.charAt(whilecounter + 3) == ')' && whilecounter + 2 + 3 == code.length())
          {
            if(code.charAt(whilecounter) == 'B' && code.charAt(whilecounter + 1) == 'I' && code.charAt(whilecounter + 2) == 'N')
            {
              whilecounter = whilecounter + 3;

              
              for(int i = address1.toInt(); i <= address2.toInt(); i++)
              {
                Serial.print("\n>: ");
                Serial.print("0x");
                Serial.print(i, HEX);
                Serial.print(": ");
                Serial.print(EEPROM.read(i), BIN);
              }
            }else if(code.charAt(whilecounter) == 'D' && code.charAt(whilecounter + 1) == 'E' && code.charAt(whilecounter + 2) == 'C')
            {
              whilecounter = whilecounter + 3;

              
              for(int i = address1.toInt(); i <= address2.toInt(); i++)
              {
                Serial.print("\n>: ");
                Serial.print(i, DEC);
                Serial.print(": ");
                Serial.print(EEPROM.read(i), DEC);
              }
            }else if(code.charAt(whilecounter) == 'H' && code.charAt(whilecounter + 1) == 'E' && code.charAt(whilecounter + 2) == 'X')
            {
              whilecounter = whilecounter + 3;

              
              for(int i = address1.toInt(); i <= address2.toInt(); i++)
              {
                Serial.print("\n>: ");
                Serial.print("0x");
                Serial.print(i, HEX);
                Serial.print(": ");
                Serial.print("0x");
                Serial.print(EEPROM.read(i), HEX);
              }
            }else
            {
              Serial.print("\n>: SYNTAX ERROR");
            }
          }else
          {
            Serial.print("\n>: SYNTAX ERROR");
          }
        }else
        {
          Serial.print("\n>: SYNTAX ERROR");
        }
      }else
      {
        Serial.print("\n>: SYNTAX ERROR");
      }
    }else
    {
      Serial.print("\n>: SYNTAX ERROR");
    }
  }
}

void EEPROM_WR_FLOAT()
{
  //SYNTAX: EEPROM_WRITE_FLOAT(VARIABLE NAME WITH 4 , ADDRESS: FROM 0 TO 41 [EACH VARIABLE OCCUPIES 12 BYTES OF EEPROM, WITH 512 BYTES BEING RESERVED FOR VARIABLES, THE OTHER 512 RESERVED FOR SIMPLE CODE], VALUE)\

  String codeStart = "EEPROM_WR_FLOAT(";

  if(code.startsWith(codeStart))
  {
    int whilecounter = 16;
    String varName;
    String address;
    String varValue;
    bool error = false;

    for(whilecounter; whilecounter <= 19; whilecounter++)
    {
      varName.concat(code.charAt(whilecounter));
    }

    //LOOP CHECKS IF ALL CHARACTERS IN VARNAME ARE NUMBERS OR IF VARIABLE CONTAINS OPERATION SYMBOLS, COMMAS, EQUAL SIGN, OR PARENTHESIS, WHICH IF TRUE WOULD BE A FORBIDDEN CONDITION
    int numbers_at_varName = 0;
    for(int i = 0; i < 4; i++)
    {
      if(numbers_at_varName >= 4)
      {
        break;
      }
      
      if(varName.charAt(i) == '1' || varName.charAt(i) == '2' || varName.charAt(i) == '3' || varName.charAt(i) == '4' || varName.charAt(i) == '5' ||
         varName.charAt(i) == '6' || varName.charAt(i) == '7' || varName.charAt(i) == '8' || varName.charAt(i) == '9' || varName.charAt(i) == '0')
      {
        numbers_at_varName = numbers_at_varName + 1;
      }else if(varName.charAt(i) == '-' || varName.charAt(i) == '+' || varName.charAt(i) == '*' || varName.charAt(i) == '/' || varName.charAt(i) == '(' || 
               varName.charAt(i) == ')' || varName.charAt(i) == ',' || varName.charAt(i) == '.' || varName.charAt(i) == ' ' || varName.charAt(i) == '=')
      {
        error = true;
      }
    }

    if(numbers_at_varName < 4 && error == false)
    {
      bool duplicateVarName = false;
      int duplicateVarAddress;

      //LOOP CHECKS FOR EXISTING VARIABLES WITH THE SAME NAME IN EEPROM
      for(int i = 0; i <= (EEPROM_CODE_START_ADDRESS/12)-1; i++)
      {
        String varNameDuplicate;

        if(duplicateVarName != false)
        {
          break;
        }

        for(int j = 0; j < 4; j++)
        {
          char varNameFromMemory = EEPROM.read((i*12) + j);

          if(varNameFromMemory == varName.charAt(j))
          {
            varNameDuplicate.concat(varNameFromMemory);
          }

          if(varName == varNameDuplicate)
          {
            duplicateVarName = true;
            duplicateVarAddress = i * 12;
            break;
          }
        }
      }
      
      if(duplicateVarName == false)
      {
        if(code.charAt(whilecounter) == ',')
        {
          whilecounter++;

          if(code.charAt(whilecounter) == '1' || code.charAt(whilecounter) == '2' || code.charAt(whilecounter) == '3' || code.charAt(whilecounter) == '4' || code.charAt(whilecounter) == '5' ||
            code.charAt(whilecounter) == '6' || code.charAt(whilecounter) == '7' || code.charAt(whilecounter) == '8' || code.charAt(whilecounter) == '9' || code.charAt(whilecounter) == '0')
          {
            while(code.charAt(whilecounter) == '1' || code.charAt(whilecounter) == '2' || code.charAt(whilecounter) == '3' || code.charAt(whilecounter) == '4' || code.charAt(whilecounter) == '5' ||
                  code.charAt(whilecounter) == '6' || code.charAt(whilecounter) == '7' || code.charAt(whilecounter) == '8' || code.charAt(whilecounter) == '9' || code.charAt(whilecounter) == '0')
            {
              address.concat(code.charAt(whilecounter));
              whilecounter++;
            }

            if(address.toInt() >= 0 && address.toInt() <= 41)
            {
              if(code.charAt(whilecounter) == ',')
              {
                whilecounter++;
                while(code.charAt(whilecounter) == '1' || code.charAt(whilecounter) == '2' || code.charAt(whilecounter) == '3' || code.charAt(whilecounter) == '4' || code.charAt(whilecounter) == '5' ||
                  code.charAt(whilecounter) == '6' || code.charAt(whilecounter) == '7' || code.charAt(whilecounter) == '8' || code.charAt(whilecounter) == '9' || code.charAt(whilecounter) == '0' || code.charAt(whilecounter) == '.')
                {
                  varValue.concat(code.charAt(whilecounter));
                  whilecounter++;
                }

                if(code.charAt(whilecounter) == ')')
                {
                  for(int i = 0; i < 12; i++)
                  {
                    EEPROM.write((address.toInt() * 12) + i, 0);
                  }

                  for(int i = 0; i < 4; i++)
                  {
                    EEPROM.write((address.toInt() * 12) + i, varName.charAt(i));
                  }

                  EEPROM.put(((address.toInt() * 12) + 4), varValue.toDouble());

                  Serial.print("\n>: STORED VARIABLE ");
                  for(int i = 0; i < 4; i++)
                  {
                    char varNameFromMemory = EEPROM.read((address.toInt()*12) + i);
                    Serial.print(varNameFromMemory);
                  }
                  Serial.print(" OF VALUE ");
                  Serial.print(varValue.toDouble());
                  Serial.print(" AT EEPROM VARIABLE ADDRESS ");
                  Serial.print(address);     
                  Serial.print(" (MEMORY ADDRESS ");
                  Serial.print(address.toInt()*12);
                  Serial.print(")");
                  Serial.print(" REMAINING EEPROM VARIABLE SPACE: ");
                  int AVAILABLE_VARIABLE_EEPROM_SPACE = 0;
                  
                  for(int i = 0; i < EEPROM_CODE_START_ADDRESS/12; i++)
                  {
                    int AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = 0;

                    for(int j = 0; j < 12; j++)
                    {
                      if(EEPROM.read((i*12) + j) == 0)
                      {
                        AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = AVAILABLE_BYTES_AT_VARIABLE_ADDRESS + 1;
                      }
                    }

                    if(AVAILABLE_BYTES_AT_VARIABLE_ADDRESS == 12)
                    {
                      AVAILABLE_VARIABLE_EEPROM_SPACE = AVAILABLE_VARIABLE_EEPROM_SPACE + 1;
                    }
                  }

                  Serial.print(AVAILABLE_VARIABLE_EEPROM_SPACE * 12);
                  Serial.print(" BYTES");
                }
              }
            }else
            {
              Serial.print("\n>: ERROR: OUT OF EEPROM MEMORY RANGE ALLOCATED FOR VARIABLES (");
              Serial.print("0 - ");
              Serial.print(EEPROM_CODE_START_ADDRESS - 1);
              Serial.print(")");
            }
          }
        }else
        {
          Serial.print("\n>: SYNTAX ERROR: MORE THAN 4 CHARACTERS FOR THE VARIABLE NAME ");
        }
      }else
      {
        Serial.print("\n>: ERROR: VARIABLE WITH THE SAME NAME ALREADY EXISTS IN EEPROM VARIABLE ADDRESS ");
        Serial.print(duplicateVarAddress/12);
        Serial.print(" (EEPROM MEMORY ADDRESS ");
        Serial.print(duplicateVarAddress);
        Serial.print(")");
      }
    }else
    {
      Serial.print("\n>: ERROR: VARIABLE NAME CANNOT CONTAIN ONLY NUMBERS, OPERATION SYMBOLS, COMMAS, PERIODS, EQUAL SIGNS, BLANK SPACES OR PARENTHESIS ");
    }
  }
}

void EEPROM_REMAINING_SPACE()
{
  Serial.print(" REMAINING EEPROM VARIABLE SPACE: ");
      int AVAILABLE_VARIABLE_EEPROM_SPACE = 0;
                  
      for(int i = 0; i < EEPROM_CODE_START_ADDRESS/12; i++)
      {
        int AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = 0;

        for(int j = 0; j < 12; j++)
        {
          if(EEPROM.read((i*12) + j) == 0)
          {
            AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = AVAILABLE_BYTES_AT_VARIABLE_ADDRESS + 1;
          }
        }

        if(AVAILABLE_BYTES_AT_VARIABLE_ADDRESS == 12)
        {
          AVAILABLE_VARIABLE_EEPROM_SPACE = AVAILABLE_VARIABLE_EEPROM_SPACE + 1;
        }
      }

      Serial.print(AVAILABLE_VARIABLE_EEPROM_SPACE * 12);
      Serial.print(" BYTES");
}

void WR_FLOAT()
{
  //THIS FUNCTION IS USED TO *DECLARE* A VARIABLE WITHOUT THE NEED TO MANUALLY DECIDE THE ADDRESS
  //SYNTAX: WR_FLOAT XXXX = YYYYYYY... 

  String codeStart = "WR_FLOAT ";
  bool error = false;
  bool duplicateVarName = false;
  int duplicateVarAddress;
  int newVarAddress;
  String varName;
  String n;
  String varValueMode;

  if(code.startsWith(codeStart))
  {
    int whilecounter = 9;
    int varNumAmount = 0;
    while(code.charAt(whilecounter) != ' ' && error == false)
    {
      if(code.charAt(whilecounter) != ',' && code.charAt(whilecounter) != '.' && code.charAt(whilecounter) != ')' && code.charAt(whilecounter) != '(' && code.charAt(whilecounter) != '+' && code.charAt(whilecounter) != '-' && code.charAt(whilecounter) != '/' && code.charAt(whilecounter) != '*' && code.charAt(whilecounter) != '=')
      {
        varName.concat(code.charAt(whilecounter));
        if(code.charAt(whilecounter) == '1' || code.charAt(whilecounter) == '2' || code.charAt(whilecounter) == '3' || code.charAt(whilecounter) == '4' || code.charAt(whilecounter) == '5' || code.charAt(whilecounter) == '6' || 
           code.charAt(whilecounter) == '7' || code.charAt(whilecounter) == '8' || code.charAt(whilecounter) == '9' || code.charAt(whilecounter) == '0')
        {
          varNumAmount = varNumAmount + 1;
        }
        whilecounter++;
      }else
      {
        error = true;
      }
    }
    whilecounter++;

    //CHECKS IF A VARIABLE WITH THE SAME NAME ALREADY EXISTS
    for(int i = 0; i <= (EEPROM_CODE_START_ADDRESS/12)-1; i++)
    {
      String varNameDuplicate;

      if(duplicateVarName != false)
      {
        break;
      }

      for(int j = 0; j < 4; j++)
      {
        char varNameFromMemory = EEPROM.read((i*12) + j);

        if(varNameFromMemory == varName.charAt(j))
        {
          varNameDuplicate.concat(varNameFromMemory);
        }

        if(varName == varNameDuplicate)
        {
          duplicateVarName = true;
          duplicateVarAddress = i * 12;
          break;
        }
      }
    }

    //THIS LOOP WILL SEARCH FOR THE FIRST AVAILABLE VARIABLE SPACE IN EEPROM
    bool emptyAddressFound = false;
    for(int i = 0; i < EEPROM_CODE_START_ADDRESS/12 && emptyAddressFound == false; i++)
    {
      int AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = 0;

      for(int j = 0; j < 12; j++)
      {
        if(EEPROM.read((i*12) + j) == 0)
        {
          AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = AVAILABLE_BYTES_AT_VARIABLE_ADDRESS + 1;
        }
      }

      if(AVAILABLE_BYTES_AT_VARIABLE_ADDRESS == 12)
      {
        newVarAddress = i * 12;
        emptyAddressFound = true;
      }
    }

    if(varName.length() > 4)
    {
      error = true;
    }else if(duplicateVarName == true)
    {
      error = true;
      Serial.print("\n>: ERROR: VARIABLE WITH THE SAME NAME ALREADY EXISTS IN EEPROM VARIABLE ADDRESS ");
      Serial.print(duplicateVarAddress/12);
      Serial.print(" (EEPROM MEMORY ADDRESS ");
      Serial.print(duplicateVarAddress);
      Serial.print(")");
    }else if(varNumAmount >= 4)
    {
      error = true;
      Serial.print("\n>: ERROR: VARIABLE CANNOT CONTAIN ONLY NUMBERS");
    }

    if(error == false && code.charAt(whilecounter) == '=' && code.charAt(whilecounter + 1) == ' ')
    {
      whilecounter = whilecounter + 2;
      for(whilecounter; whilecounter < code.length()-1; whilecounter++)
      {
        n.concat(code.charAt(whilecounter));
      }
    }

    //Will check if n is a FUNC command
    String funcStart = "FUNC(";
    if(n.startsWith(funcStart) && n.charAt(n.length() - 1) == ')' && error == false)
    {
      REPL = false;
      code = n;
      code.concat(" ");
      if(FUNC() != NULL)
      {
        varValueMode = "FUNC";
        n = FUNC();

        if(error == false)
        {
          for(int i = 0; i < 4; i++)
          {
            EEPROM.write(newVarAddress + i, varName.charAt(i));
          }
        }

        EEPROM.put(newVarAddress + 4, n.toDouble());
        
        Serial.print("\n>: STORED VARIABLE ");
        for(int i = 0; i < 4; i++)
        {
          char varNameFromMemory = EEPROM.read((newVarAddress) + i);
          Serial.print(varNameFromMemory);
        }

        Serial.print(" OF VALUE ");
        Serial.print(n.toDouble());
        Serial.print(" AT EEPROM VARIABLE ADDRESS ");
        Serial.print(newVarAddress / 12);     
        Serial.print(" (MEMORY ADDRESS ");
        Serial.print(newVarAddress);
        Serial.print(")");
        EEPROM_REMAINING_SPACE();
      }
      
      REPL = true;
    }else if(error == false)
    {
      //ANALYZES n STRING TO DETERMINE IF IT IS A VARIABLE OR A CONSTANT NUMBER
      if(VALUE_ANALYZER(n) == "error")
      {
        error = true;
      }
      varValueMode = VALUE_ANALYZER(n);
    }

    if(error == false && varValueMode == "CONST_NUM")
    {
      if(error == false)
      {
        for(int i = 0; i < 4; i++)
        {
          EEPROM.write(newVarAddress + i, varName.charAt(i));
        }
      }

      EEPROM.put(newVarAddress + 4, n.toDouble());
      
      Serial.print("\n>: STORED VARIABLE ");
      for(int i = 0; i < 4; i++)
      {
        char varNameFromMemory = EEPROM.read((newVarAddress) + i);
        Serial.print(varNameFromMemory);
      }

      Serial.print(" OF VALUE ");
      Serial.print(n.toDouble());
      Serial.print(" AT EEPROM VARIABLE ADDRESS ");
      Serial.print(newVarAddress / 12);     
      Serial.print(" (MEMORY ADDRESS ");
      Serial.print(newVarAddress);
      Serial.print(")");
      EEPROM_REMAINING_SPACE();
      
    }else if(error == false && varValueMode == "VAR")
    {
      int VarAddress;
      double varValue;
      bool VarName = false;

      //CHECKS IF A VARIABLE WITH THE SAME NAME ALREADY EXISTS
      for(int i = 0; i <= (EEPROM_CODE_START_ADDRESS/12)-1; i++)
      {
        String varNameDuplicate;

        if(duplicateVarName != false)
        {
          break;
        }

        for(int j = 0; j < 4; j++)
        {
          char varNameFromMemory = EEPROM.read((i*12) + j);

          if(varNameFromMemory == n.charAt(j))
          {
            varNameDuplicate.concat(varNameFromMemory);
          }

          if(n == varNameDuplicate)
          {
            VarName = true;
            VarAddress = i * 12;
            break;
          }
        }
      }

      if(VarName == true)
      {
        if(error == false)
        {
          for(int i = 0; i < 4; i++)
          {
            EEPROM.write(newVarAddress + i, varName.charAt(i));
          }
        }

        EEPROM.get(VarAddress + 4, varValue);
        EEPROM.put(newVarAddress + 4, varValue);

        Serial.print("\n>: STORED VARIABLE ");
        for(int i = 0; i < 4; i++)
        {
          char varNameFromMemory = EEPROM.read((newVarAddress) + i);
          Serial.print(varNameFromMemory);
        }

        Serial.print(" OF VALUE ");
        Serial.print(varValue);
        Serial.print(" AT EEPROM VARIABLE ADDRESS ");
        Serial.print(newVarAddress / 12);     
        Serial.print(" (MEMORY ADDRESS ");
        Serial.print(newVarAddress);
        Serial.print(")");
        EEPROM_REMAINING_SPACE();

      }else
      {
        error = true;
        Serial.print("\n>: ERROR: VARIABLE NOT FOUND ");
      }
    }
  }
}

void EEPROM_INFO()
{
  //SYNTAX: EEPROM_INFO()
  String codeStart = "EEPROM_INFO()";

  if(code.startsWith(codeStart))
  {
    Serial.print("\n>: EEPROM: ");
    Serial.print(EEPROM.length());
    Serial.print(" BYTES");
    Serial.print(" | AVAILABLE VARIABLE EEPROM SPACE: ");
    int AVAILABLE_VARIABLE_EEPROM_SPACE = 0;
    
    for(int i = 0; i < EEPROM_CODE_START_ADDRESS/12; i++)
    {
      int AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = 0;

      for(int j = 0; j < 12; j++)
      {
        if(EEPROM.read((i*12) + j) == 0)
        {
          AVAILABLE_BYTES_AT_VARIABLE_ADDRESS = AVAILABLE_BYTES_AT_VARIABLE_ADDRESS + 1;
        }
      }

      if(AVAILABLE_BYTES_AT_VARIABLE_ADDRESS == 12)
      {
        AVAILABLE_VARIABLE_EEPROM_SPACE = AVAILABLE_VARIABLE_EEPROM_SPACE + 1;
      }
    }

    Serial.print(AVAILABLE_VARIABLE_EEPROM_SPACE * 12);
    Serial.print(" BYTES");
    Serial.print(" | AVAILABLE VARIABLE EEPROM SPACE: ");
    Serial.print(EEPROM.length());
    Serial.print("\n");
  }
}

void DIGITAL_WRITE()
{
  //SYNTAX: DIGITAL_WRITE(PINNUMBER, HIGH/LOW)
  String codeStart = "DIGITAL_WRITE(";
  
  if(code.startsWith(codeStart) && code.charAt(code.length() - 2) == ')')
  {
    bool error = false;
    String pinLevel;
    int whilecounter = 14;
    String digitalPin;

    while(code.charAt(whilecounter) != ',' && whilecounter <= code.length())
    {
      digitalPin.concat(code.charAt(whilecounter));
      whilecounter++;
    }
    whilecounter++;

    for(int i = 0; i < digitalPin.length(); i++)
    {
      if(digitalPin.charAt(i) != '1' && digitalPin.charAt(i) != '2' && digitalPin.charAt(i) != '3' && digitalPin.charAt(i) != '4' && digitalPin.charAt(i) != '5' && 
         digitalPin.charAt(i) != '6' && digitalPin.charAt(i) != '7' && digitalPin.charAt(i) != '8' && digitalPin.charAt(i) != '9' && digitalPin.charAt(i) != '0')
      {
        error = true;
      }
    }

    while(code.charAt(whilecounter) != ')' && whilecounter <= code.length())
    {
      pinLevel.concat(code.charAt(whilecounter));
      whilecounter++;
    }

    if(error == false && digitalPin.toInt() < 30)
    {
      if(pinLevel == "HIGH")
      {
        digitalWrite(digitalPin.toInt(), HIGH);
        Serial.print("\n>: PIN ");
        Serial.print(digitalPin);
        Serial.print(" SET TO ");
        Serial.print(pinLevel);

      }else if(pinLevel == "LOW")
      {
        digitalWrite(digitalPin.toInt(), LOW);
        Serial.print("\n>: PIN ");
        Serial.print(digitalPin);
        Serial.print(" SET TO ");
        Serial.print(pinLevel);
      }else
      {
        Serial.print("\n>: PIN LEVEL SYNTAX ERROR ");
      }
    }else
    {
      Serial.print("\n>: SYNTAX ERROR ");
    }
  }
}

void loop() {
  while (Serial.available() == 0) {}     
  code = Serial.readString();
  
  EEPROM_ERASE();
  EEPROM_INFO();
  EEPROM_WR_FLOAT();
  EEPROM_READ_FROM();
  FUNC();
  WR_FLOAT();
  DIGITAL_WRITE();
}
