#ifndef HEADER_FILE_API_ASSERT_B07BE67D_925C_4201_92DB_A20EBE710DE1
#define HEADER_FILE_API_ASSERT_B07BE67D_925C_4201_92DB_A20EBE710DE1


#include <stdio.h>
#include <string>


namespace iridium::io::fs::implementation {


int assertOK(const int &result, const std::string &message);


::FILE *assertOK(::FILE *result, const std::string &message);


} // iridium::io::fs::implementation


#endif // HEADER_FILE_API_ASSERT_B07BE67D_925C_4201_92DB_A20EBE710DE1
