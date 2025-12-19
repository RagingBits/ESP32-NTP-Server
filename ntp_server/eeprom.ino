

#include <SPIFFS.h>
#include "EEPROM.h"



/* EPROM definitions and variables -------------------------------------------- */
#define SSID_ADDR                   (0)
#define SSID_MAX_LEN                (50)

#define PASS_ADDR                   (SSID_ADDR+SSID_MAX_LEN)
#define PASS_MAX_LEN                (50)

#define OFFSET_ADDR                 (PASS_ADDR+PASS_MAX_LEN)
#define OFFSET_MAX_LEN              (1)


#define EEPROM_LEN                  (OFFSET_ADDR+OFFSET_MAX_LEN)




typedef struct
{
    uint32_t initialised:1U;
    uint32_t unused:31U;
}eeprom_client_flags_t;


#define EEPROM_SSID         0
#define EEPROM_PASS         1
#define EEPROM_OFFSET       2
#define EEPROM_TOTAL        3

#define EEPROM_SSID_LEN     SSID_MAX_LEN
#define EEPROM_PASS_LEN     PASS_MAX_LEN
#define EEPROM_OFFSET_LEN   OFFSET_MAX_LEN

/* EEPROM Global Variables ---------------------------------------------------- */
uint8_t eeprom_ssid[SSID_MAX_LEN];
uint8_t eeprom_paraphrase[PASS_MAX_LEN];
uint8_t eeprom_offset[OFFSET_MAX_LEN];
eeprom_client_flags_t eeprom_client_flags = {0U};


/* EEPROM Global Constants ---------------------------------------------------- */
const uint8_t eeprom_lengths[EEPROM_TOTAL] = 
{
    SSID_MAX_LEN,
    PASS_MAX_LEN,
    OFFSET_MAX_LEN
};

const uint16_t eeprom_addresses[EEPROM_TOTAL] = 
{
    SSID_ADDR,
    PASS_ADDR,
    OFFSET_ADDR
};


uint8_t *const eeprom_variables[EEPROM_TOTAL] = 
{
    &eeprom_ssid[0U],
    &eeprom_paraphrase[0U],
    &eeprom_offset[0]
};

uint8_t const eeprom_name[EEPROM_TOTAL][24] = 
{
    "SSID",
    "PASS",
    "OFFSET"
};

bool string_compare(char *a, char *b, uint32_t len)
{
    bool equal = false;
    uint32_t counter = len;
    
    while((0U != *a) && (0U != *b) && (*a == *b) && (counter > 0))
    {
        counter--;
        a++;
        b++;
    }  
  
    if((0U == counter) && (0U != len))
    {
        equal = true;
    }
    
    return equal;
}

/* EEPROM External Functions --------------------------------------------------- */

void EepromStart(void)
{
    uint8_t counter = 0U;
  
    Serial.println(" ");
    Serial.println("EEPROM start"); 
    if(0U == eeprom_client_flags.initialised)
    {
        if(!EEPROM.begin(EEPROM_LEN))
        {
          Serial.println("failed!!!");  
        }
        eeprom_client_flags.initialised = !0U;
    }
    
    if(0U != eeprom_client_flags.initialised)
    {
    
        Serial.println("    Loading data... "); 
        while(counter < EEPROM_TOTAL)
        {
            EEPROM.readBytes(eeprom_addresses[counter],(void*)eeprom_variables[counter],(size_t)eeprom_lengths[counter]);
            Serial.print("    "); Serial.print((char*)eeprom_name[counter]);  Serial.print(" : "); Serial.println((char*)eeprom_variables[counter]);
            counter++;
        }

        Serial.println("done."); 
    } 
    
}

void EepromClear(void)
{
    uint8_t counter = 1U;

    Serial.println(" ");
    Serial.println("EEPROM erase request.");
    Serial.print("    EEPROM erasing...");

    while(counter < EEPROM_TOTAL)
    {
        
        uint16_t counter2 = eeprom_lengths[counter];
        while(0U != counter2)
        {
            counter2--;
            eeprom_variables[counter][counter2] = 0U;
        }

        EEPROM.writeBytes(eeprom_addresses[counter],(void*)eeprom_variables[counter],(size_t)eeprom_lengths[counter]); 
        counter++;
    }
    
    EEPROM.commit();
    
    Serial.println(" done.");
  
}

void EepromRead(uint8_t data_type, uint8_t *data_in, uint8_t *data_length)
{
    if(0U != eeprom_client_flags.initialised)
    {
        uint8_t counter = *data_length;

        if(counter > eeprom_lengths[data_type])
        {
            counter = *data_length = eeprom_lengths[data_type];
        }
        
        while(0U != counter)
        {
            counter--;
            data_in[counter] = eeprom_variables[data_type][counter];
        }
    }
}

void EepromWrite(uint8_t data_type, uint8_t *data_in, uint8_t data_length)
{
    if(0U != eeprom_client_flags.initialised)
    {
      Serial.println(" "); 
      Serial.println("EEPROM writting "); 
      uint8_t counter = eeprom_lengths[data_type];
      uint8_t counter2 = 0U;
      
      bool result = string_compare((char*)data_in,(char*)eeprom_variables[data_type],eeprom_lengths[data_type]);   
      
      if(!result)
      {
          while(0U != counter)
          {
              counter --;
              eeprom_variables[data_type][counter] = 0U;
          }
      
          counter = data_length;
          if(counter > eeprom_lengths[data_type])
          {
              counter = eeprom_lengths[data_type];
          }
          counter2 = counter;
          
          while(0U != counter)
          {
              counter--;
              eeprom_variables[data_type][counter] = data_in[counter];
          }
          
          Serial.print("    "); Serial.print((char*)eeprom_name[data_type]);  Serial.print(" : "); Serial.println((char*)eeprom_variables[data_type]);
          EEPROM.writeBytes(eeprom_addresses[data_type],(void*)eeprom_variables[data_type], (size_t)counter2); 
          EEPROM.commit();
      }
      else
      {
          Serial.println("    Already the same value.");
      }
    }
}
