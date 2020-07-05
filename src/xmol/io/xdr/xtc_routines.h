/* A subset of libxdrf.c library (https://github.com/Pappulab/xdrf)

 Originally written by Frans van Hoesel, hoesel@chem.rug.nl (c) 1995
 Updated by Alex Holehouse (c) 2014
 Adapted by Sergei Izmailov (c) 2020

*/


#include <array>

namespace xmol::io::xdr::xtc {

/*____________________________________________________________________________
 |
 | sendints - send a small set of small integers in compressed format
 |
 | this routine is used internally by xdr3dfcoord, to send a set of
 | small integers to the buffer.
 | Multiplication with fixed (specified maximum ) sizes is used to get
 | to one big, multibyte integer. Allthough the routine could be
 | modified to handle sizes bigger than 16777216, or more than just
 | a few integers, this is not done, because the gain in compression
 | isn't worth the effort. Note that overflowing the multiplication
 | or the byte buffer (32 bytes) is unchecked and causes bad results.
 |
 */

void sendints(int buf[], const int num_of_ints, const int num_of_bits,
                     unsigned int sizes[], unsigned int nums[]);

/*____________________________________________________________________________
 |
 | sendbits - encode num into buf using the specified number of bits
 |
 | This routines appends the value of num to the bits already present in
 | the array buf. You need to give it the number of bits to use and you
 | better make sure that this number of bits is enough to hold the value
 | Also num must be positive.
 |
*/

void sendbits(int buf[], int num_of_bits, int num);

/**__________________________________________________________________________
 |
 | receivebits - decode number from buf using specified number of bits
 |
 | extract the number of bits from the array buf and construct an integer
 | from it. Return that value.
 |
*/
int receivebits(int buf[], int num_of_bits);

/**___________________________________________________________________________
 |
 | receiveints - decode 'small' integers from the buf array
 |
 | this routine is the inverse from sendints() and decodes the small integers
 | written to buf by calculating the remainder and doing divisions with
 | the given sizes[]. You need to specify the total number of bits to be
 | used from buf in num_of_bits.
 |
*/
void receiveints(int buf[], const int num_of_ints, int num_of_bits, unsigned int sizes[], int nums[]);

/**________________________________________________________________________
 |
 | sizeofint - calculate bitsize of an integer
 |
 | return the number of bits needed to store an integer with given max size
 |
*/
int sizeofint(const int size);

/**_________________________________________________________________________
 |
 | sizeofints - calculate 'bitsize' of compressed ints
 |
 | given the number of small unsigned integers and the maximum value
 | return the number of bits needed to read or write them with the
 | routines receiveints and sendints. You need this parameter when
 | calling these routines. Note that for many calls I can use
 | the variable 'smallidx' which is exactly the number of bits, and
 | So I don't need to call 'sizeofints for those calls.
*/
int sizeofints(const int num_of_ints, unsigned int sizes[]);

[[maybe_unused]] constexpr std::array magicints{
    0,       0,       0,       0,       0,        0,        0,       0,       0,       8,       10,
    12,      16,      20,      25,      32,       40,       50,      64,      80,      101,     128,
    161,     203,     256,     322,     406,      512,      645,     812,     1024,    1290,    1625,
    2048,    2580,    3250,    4096,    5060,     6501,     8192,    10321,   13003,   16384,   20642,
    26007,   32768,   41285,   52015,   65536,    82570,    104031,  131072,  165140,  208063,  262144,
    330280,  416127,  524287,  660561,  832255,   1048576,  1321122, 1664510, 2097152, 2642245, 3329021,
    4194304, 5284491, 6658042, 8388607, 10568983, 13316085, 16777216};

[[maybe_unused]] constexpr int FIRSTIDX = 9;

constexpr int SQR(int x) { return x * x; };

} // namespace xmol::io::xdr::xtc