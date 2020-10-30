#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>

int validateCard(int card_nr[], int number_of_digits);
int calculateProductsDigits(int number_of_digits, int card_nr[]);
int checkCardCompany(int card_nr[], int digit_counter);


int main(void)
{
    /*
     * This program checks if credit card is valid and what's the type of card.
     * It can detect Visa, Mastercard and American Express cards.
     */

    long card_nr;
    long card_nr_copy;
    int digit_counter;


    //Getting card number from user and checking it's lenght.


    card_nr = get_long("Please enter card number here:\n");

    //Checking lenght of card number. Accepted lenght: 13, 15, 16.
    if (card_nr < 10000000000000000 && card_nr > 1000000000000000)
    {
        digit_counter = 16;
    }
    else if (card_nr < 1000000000000000 && card_nr > 100000000000000)
    {
        digit_counter = 15;
    }
    else if (card_nr < 10000000000000 && card_nr > 1000000000000)
    {
        digit_counter = 13;
    }
    else
    {
        printf("INVALID\n");
        exit(0);
    }


    //Copying card number for further operations.
    card_nr_copy = card_nr;


    //Creating arrays to store card number's digits.
    int inverted_digit_storage[digit_counter];
    int digit_storage[digit_counter];

    //Assigning digits to array.
    for (int i = 0; i < digit_counter; i++)
    {
        inverted_digit_storage[i] = card_nr % 10;
        card_nr /= 10;
    }

    //Reversing array.
    for (int i = 0; i < digit_counter; i++)
    {
        digit_storage[i] = inverted_digit_storage[digit_counter - 1 - i];
    }
    //printf("first digit: %d\n", digit_storage[0]);
    //printf("last digit: %d\n", digit_storage[digit_counter-1]);


    //Checking card with Luhn's Algorithm
    int status = validateCard(digit_storage, digit_counter);
    //printf("status: %i\n", status);

    //Checking card company.
    int card_type;
    if (checkCardCompany(digit_storage, digit_counter) == 1)
    {
        printf("VISA\n");
    }
    else if (checkCardCompany(digit_storage, digit_counter) == 2)
    {
        printf("MASTERCARD\n");
    }
    else if (checkCardCompany(digit_storage, digit_counter) == 3)
    {
        printf("AMEX\n");
    }
    else
    {
        printf("INVALID\n");
        exit(0);
    }


}


int validateCard(int card_nr[], int number_of_digits)
{

    /*
     * This function checks if the total modulo 10 is congruent to 0,
     * If yes, then the card number is valid.
     */
    int status = 2;
    int total_sum;

    total_sum = calculateProductsDigits(number_of_digits, card_nr);
    //printf("total sum: %i\n", total_sum);

    if (total_sum % 10 == 0)
    {
        status = 0;
    }
    else
    {
        status = 1;
        //printf("status inside validate card: %i\n", status);
        printf("INVALID\n");
        exit(0);
    }

    return status;
}



int calculateProductsDigits(int number_of_digits, int card_nr[])
{

    /*
     * This function determines if credit card's number is valid
     * using Luhn's Algorithm.
     */

    int total_sum = 0;
    int sum_of_odds = 0;
    int sum_of_evens = 0;


    /*
     * Calculating multiplication of every other digit by 2,
     * starting with the number's second-to-last digit,
     * and then adding those products’ digits together.
     */
    for (int i = 0; i < number_of_digits; i += 2)
    {
        if (card_nr[i] * 2 > 9)
        {
            /*
             * Variables 'a' and 'b' are respectively a decimal
             * and unit digit of a number represented by 'card_nr[i]'
             * when 'card_nr[i] * 2' > 9.
             * That rule is implemented here to fulfill rules of Luhn's Algorithm.
             *
             * Variable 'temp' is temporary to store sum of variables 'a' and 'b'.
             */
            int a = (card_nr[i] * 2) % 10;
            //printf("\n***********\n");
            //printf("card_nr[%i] is %i\n", i, card_nr[i]);
            //printf("a[%i]: %i\n", i, a);
            int b = (card_nr[i] * 2 - (card_nr[i] * 2) % 10) / 10;
            //printf("b[%i]: %i\n", i, b);
            int temp = a + b;
            //printf("temp[%i]: %i\n", i, temp);

            sum_of_odds = sum_of_odds + temp;
            //printf("sum of odds: %i\n\n", sum_of_odds);
        }
        else
        {
            //printf("\n***********\n");
            //printf("card_nr[%i] is %i\n", i, card_nr[i]);
            sum_of_odds = sum_of_odds + (card_nr[i] * 2);
            //printf("sum of odds: %i\n\n", sum_of_odds);
        }
    }


    /*
     * Calculating sum of the digits that weren't multiplied by 2.
     */
    for (int i = 1; i < number_of_digits; i += 2)
    {
        sum_of_evens = sum_of_evens + card_nr[i];
        //printf("\n#############\n");
        //printf("card_nr[%i] is %i\n", i, card_nr[i]);
        //printf("sum of evens: %i\n\n", sum_of_evens);
    }

    total_sum = sum_of_odds + sum_of_evens;
    //printf("sum of odds: %i\n", sum_of_odds);
    //printf("sum of evens: %i\n", sum_of_evens);
    //printf("total sum inside function: %i\n\n", total_sum);
    return total_sum;
}



int checkCardCompany(int card_nr[], int digit_counter)
{

    /*
     * This function checks what's the credit card's company,
     * based on first digits and number of digits.
     */

    int card_type;

    //VISA
    if ((digit_counter == 13 && card_nr[0] == 4) || (digit_counter == 16 && card_nr[0] == 4))
    {
        card_type = 1;
        //printf("VISA\n");
    }

    //MASTERCARD
    else if ((digit_counter == 16 && card_nr[0] == 5 && card_nr[1] == 1) || (digit_counter == 16 && card_nr[0] == 5 && card_nr[1] == 2)
             || (digit_counter == 16 && card_nr[0] == 5 && card_nr[1] == 3) || (digit_counter == 16 && card_nr[0] == 5 && card_nr[1] == 4)
             || (digit_counter == 16 && card_nr[0] == 5 && card_nr[1] == 5))
    {
        card_type = 2;
        //printf("MASTERCARD\n");
    }

    //AMEX
    else if ((digit_counter == 15 && card_nr[0] == 3 && card_nr[1] == 4) || (digit_counter == 15 && card_nr[0] == 3 && card_nr[1] == 7))
    {
        card_type = 3;
        //printf("AMEX\n");
    }
    else
    {
        card_type = 0;
    }

    return card_type;

}