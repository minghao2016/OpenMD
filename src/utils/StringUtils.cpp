 /*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Acknowledgement of the program authors must be made in any
 *    publication of scientific results based in part on use of the
 *    program.  An acceptable form of acknowledgement is citation of
 *    the article in which the program was described (Matthew
 *    A. Meineke, Charles F. Vardeman II, Teng Lin, Christopher
 *    J. Fennell and J. Daniel Gezelter, "OOPSE: An Object-Oriented
 *    Parallel Simulation Engine for Molecular Dynamics,"
 *    J. Comput. Chem. 26, pp. 252-271 (2005))
 *
 * 2. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. All express or implied conditions, representations and
 * warranties, including any implied warranty of merchantability,
 * fitness for a particular purpose or non-infringement, are hereby
 * excluded.  The University of Notre Dame and its licensors shall not
 * be liable for any damages suffered by licensee as a result of
 * using, modifying or distributing the software or its
 * derivatives. In no event will the University of Notre Dame or its
 * licensors be liable for any lost revenue, profit or data, or for
 * direct, indirect, special, consequential, incidental or punitive
 * damages, however caused and regardless of the theory of liability,
 * arising out of the use of or inability to use software, even if the
 * University of Notre Dame has been advised of the possibility of
 * such damages.
 */
 
#include "utils/StringUtils.hpp"

namespace oopse {
std::string UpperCase(const std::string& S) {
  std::string uc = S;
  unsigned int n = uc.size();
  for (unsigned int j = 0; j < n; j++) {   
    char sj = uc[j];
    if (sj >= 'a' && sj <= 'z') uc[j] = (char)(sj - ('a' - 'A'));
  }
  return uc;
}

std::string LowerCase(const std::string& S) {
  std::string lc = S;
  unsigned int n = lc.size();
  for (unsigned int j = 0; j < n; j++) {
    char sj = lc[j];
    if (sj >= 'A' && sj <= 'Z') lc[j] = (char)(sj + ('a' - 'A'));
  }
  return lc;
}

char* trimSpaces(char *str) {
  size_t len;
  char *right, *left;

  if (strlen(str) == 0) return(str);
  
  /* Trim whitespace from left side */
  for (left = str; isspace(*left); left++);

  /* Trim whitespace from right side */
  if ((len = strlen(left)))
    {
      right = left + (len - 1);
      
      while (isspace(*right))
        {
          *right = '\0';
          right--;
        }
    }

  /* Only do the str copy if there were spaces to the left */
  if (left != str)
    strcpy(str, left);
  
  return (str);
}

int findBegin(std::istream &theStream, char* startText ){
  const int MAXLEN = 1024;
  char readLine[MAXLEN];   
  int foundText = 0;
  int lineNum;
  char* the_token;
  char* eof_test;

  // rewind the stream
  theStream.seekg (0, std::ios::beg);
  lineNum = 0;
  
  if (!theStream.eof()) {
    theStream.getline(readLine, MAXLEN);
    lineNum++;
  } else {
    printf( "Error fast forwarding stream: stream is empty.\n");
    return -1;
  }
  
  while ( !foundText ) {
    
    if (theStream.eof()) {
      printf("Error fast forwarding stream at line %d: "
             "stream ended unexpectedly.\n", lineNum);
      return -1;
    }
    
    the_token = strtok( readLine, " ,;\t" );
    if ( the_token != NULL)
      if (!strcasecmp("begin", the_token)) {
	the_token = strtok( NULL, " ,;\t" );
	if ( the_token != NULL){
	  foundText = !strcasecmp( startText, the_token );
	}
      }
    
    if (!foundText) {
      if (!theStream.eof()) {
        theStream.getline(readLine, MAXLEN);
        lineNum++;
      } else {
        printf( "Error fast forwarding stream at line %d: "
                "stream ended unexpectedly.\n", lineNum);
	return -1;
      }
    }
  }
  return lineNum;
}

int countTokens(char *line, char *delimiters) {
  /* PURPOSE: RETURN A COUNT OF THE NUMBER OF TOKENS ON THE LINE. */
  
  char *working_line;   /* WORKING COPY OF LINE. */
  int ntokens;          /* NUMBER OF TOKENS FOUND IN LINE. */
  char *strtok_ptr;     /* POINTER FOR STRTOK. */

  strtok_ptr= working_line= strdup(line);

  ntokens=0;
  while (strtok(strtok_ptr,delimiters)!=NULL)
    {
      ntokens++;
      strtok_ptr=NULL;
    }

  free(working_line);
  return(ntokens);
}

int isEndLine(char *line) {
  char *working_line;
  char *foo;
  
  working_line = strdup(line);
  
  foo = strtok(working_line, " ,;\t");

  if (foo != NULL) {

    if (!strcasecmp(foo, "end")) return 1;

  }
 
 return 0;
}


std::string getPrefix(const std::string& str) {
  return str.substr(0, str.rfind('.'));
}

std::string getSuffix(const std::string& str) {
    return str.substr(0, str.find('.'));
}

}
