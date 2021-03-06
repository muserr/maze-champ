//Input relevant port numbers
//STRICTLY USE ONLY PINS A0 to A5
#define ALREAD_PIN_1 A1
#define ALREAD_PIN_2 A2

#define BAUD_RATE 9600

#define SAMPLE_SIZE 200
#define NO_OF_MAX 10

#define VONSTANT 5 / 1023

int al_value_1;
int al_value_2;

int digital_val_1;
int digital_val_2;

//Declare 2 array for storing of data values
int arr_val_1[SAMPLE_SIZE] = {0};
int arr_val_2[SAMPLE_SIZE] = {0};

//Declare 2 array for storing of 10 maximum data values
int max_arr_1[NO_OF_MAX] = {0};
int max_arr_2[NO_OF_MAX] = {0};


//In order of program function calls.
void audio_read();
void ADC_convert(int arr_val_1[], int arr_val_2[]);
void maxima_maximus(int arr_val_1[], int arr_val_2[]);
int average(int arr[]);
void threshold_check(int max_avg_1, int max_avg_2);



void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {
}

//FUNCTION: To start off audio reading
void audio_read()
{
  int i;
  int j;
  int max_avg_1;
  int max_avg_2;
  
  //300 Hz: Collecting SAMPLE_SIZE = 200 samples each
  for (i = 0; i < SAMPLE_SIZE; i += 1)
  {
     //For the case of frequency = 300 Hz
     arr_val_1[i] = analogRead(ALREAD_PIN_1);

     //Delay time in MILLIseconds: specific to each frequency
     //Every full wave = 3.333 ms
     delay(1.1111); //Let's go with 1/3 of the period.. no reason why just an estimation
  }

  //3000 Hz: Collecting SAMPLE_SIZE = 200 samples each
  for (i = 0; i < SAMPLE_SIZE; i += 1)
  {
     //For the case of frequency = 300 Hz
     arr_val_2[i] = analogRead(ALREAD_PIN_2);

     //Delay time in MILLIseconds: specific to each frequency
     //Every full wave = 0.333 ms
     delay(0.1111); //Let's go with 1/3 of the period.. no reason why just an estimation lol
  }


  //Call ADC converter function
  ADC_convert(arr_val_1, arr_val_2);

  //Find max
  maxima_maximus(arr_val_1, arr_val_2);

  //Find average of maximum
  max_avg_1 = average(arr_val_1);
  max_avg_2 = average(arr_val_2);

  //Comparing both values against requirement and turn accordingly
  threshold_check(max_avg_1, max_avg_2);
  
}


//FUNCTION: Converts ADC linear value into its voltage equivalent
void ADC_convert(int arr_val_1[], int arr_val_2[])
{
  //Replace every ADC linear value with its corresponding voltage between 0 and 5 volts
  int i;

  for (i = 0; i < SAMPLE_SIZE; i += 1)
  {
    //Convert by multiplying by its 10 bit voltage constant
    arr_val_1[i] *= VONSTANT;
    
    arr_val_2[i] *= VONSTANT;
  }
}


//FUNCTION: Finds maximum value within a set and stores it in array customed for maximum values
void maxima_maximus(int arr_val_1[], int arr_val_2[])
{
  int i;
  int j = 0;
  int k = 10;
  int maxima;
  
  //For every interval of 20 counts (Depending on the number of samples taken)
  for (i = 0; i < NO_OF_MAX; i += 1)
  {
    //Initialize maxima to 0 before every iteration
    maxima = 0;

    //Input array name, lower bound of array_pos, upper bound of array_pos
    max_arr_1[i] = maxima_search(arr_val_1, j, j + k);

    max_arr_2[i] = maxima_search(arr_val_2, j, j + k);

    j += 10;
    k += 10;  
  }
}


//FUNCTION: Finding maximum values in a set
int maxima_search(const int arr[], int lower, int higher)
{
  int maxima;
  int i;

  maxima = 0;

  for (i = lower; i < higher; i += 1)
  {
    if (arr[i] > maxima)
    {
      maxima = arr[i];
    }
  }

  return maxima;
}


//FUNCTION: Within the array of maximum values, obtain its average value
int average(int arr[])
{
  int i;
  int sum = 0;

  for (i = 0; i < NO_OF_MAX; i += 1)
  {
    sum += arr[i];
  }

  //Calculate average value
  return sum / NO_OF_MAX;
}


//FUNCTION: Comparing one max_average value with another, and perform task specific to its volume differences
void threshold_check(int max_avg_1, int max_avg_2)
{
  //Instead of the calculated 1 / sqrt(10), approx it to '2 point sth' instead
  if (max_avg_1 >= 2.5 * max_avg_2)
  {
    //The 300 Hz audio is louder (This depends on initially what we define the avg_1 to represent)

    /**
     * Perform Left-turn
     */
  }

  else if (max_avg_2 >= 2.5 * max_avg_1)
  {
    //The 3000 Hz audio is louder

    /**
     * PERFORM RIGHT TURN
     */
  }

  //1.5 as an estimation of approximately equal volume (plus minus abit)
  else if   ((max_avg_1 < 1.5 * max_avg_2) && (max_avg_2 < 1.5 * max_avg_1))
  {
    /**
     * 180 DEGREES OF TURNING
     */
  }
}
