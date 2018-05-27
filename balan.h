#ifndef BALAN_H_INCLUDED
#define BALAN_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include "util.h"

const int ENV = 0;
const int DEVELOP = 1;
const int PRODUCT = 0;

const double PI = 3.14159265;
const int MAX_ELE = 30;
const int MAX_STR = 100;
const int TRUE = 1;
const int FALSE = 0;
//char elementMath[200][200];
//int lengthArrEle = 0;

// NOTE: Change this!!!
const int TOTAL_OPE = 13;
const int MAX_LEN_OPE = 7;  // NOTE: strlen()

int isOperator(char *ope) {
  char operators[13][7] = {"+", "-", "*", "/", "(", ")", "~", "sin", "cos", "tan", "arcsin", "sqrt", "log"};

  for (int i=0; i<TOTAL_OPE; i++) {
    if (strcmp(ope, operators[i]) == 0) { return TRUE; }
  }

  return FALSE;
}

int getPriority(char *ope) {
  if (ENV == DEVELOP) { printf("\n\n\t>> getPriority: %s", ope); }

  int p = 1;
  if (strcmp(ope, "+") == 0 || strcmp(ope, "-") == 0) { return p; }
  p++; if (strcmp(ope, "*") == 0 || strcmp(ope, "/") == 0) { return p; }
  p++; if (strcmp(ope, "^") == 0) { return p; }
  p++; if (strcmp(ope, "~") == 0) { return p; }
  p++; if (strcmp(ope, "sin") == 0
      || strcmp(ope, "cos") == 0
      || strcmp(ope, "tan") == 0
      || strcmp(ope, "arcsin") == 0
      || strcmp(ope, "arccos") == 0
      || strcmp(ope, "arctan") == 0
      || strcmp(ope, "log") == 0
  ) { return p; }
  p++; if (strcmp(ope, "sqrt") == 0) { return p; }

  return 0;
}

int isDigital(char c) {
	if (c == '.') { return TRUE; }
	if (c >= '0' && c <= '9') { return TRUE; }
	return FALSE;
}

int isNumber(char *n) {
  for (int i=0; i<strlen(n); i++) {
    if (n[i] == '.') { continue; }
    if (n[i] < '0' || n[i] > '9') { return FALSE; }
  }
  return TRUE;
}

char* standardize(char *math) {
	char *sMath = trim(math);
	// TODO: Remove double whitespace to one
  return sMath;
}

char* processInput(char *sMath) {
  char *result = calloc(MAX_STR, sizeof (char));
	char *tmp = calloc(MAX_STR, sizeof (char));
	int iTmp = 0;
	if (ENV == DEVELOP) { printf("\n\n\t>> processInput len %s: %d\n\n", sMath, strlen(sMath)); }

	// NOTE: From sMath[] ---> elementMath[][]
	// "11+2.5" ---> elementMath[0] = "11", elementMath[1] = "2.5"
	for (int i=0; i<strlen(sMath); i++) {
		if (isDigital(sMath[i]) == FALSE) {
      if (sMath[i] == ' ') { continue; }
			if (strlen(tmp) != 0) {
        strcat(result, tmp); strcat(result, ";");
			}

      // NOTE: Is operator but not a word
      char *pChar = calloc(MAX_LEN_OPE, sizeof (char));
      pChar[0] = sMath[i];
      if (isOperator(pChar) == TRUE) {
        result[strlen(result)] = sMath[i]; result[strlen(result)] = ';';
      } else {
        // NOTE: Is word (sin, cos, tan) and not a operator
        do {
          i++; pChar[strlen(pChar)] = sMath[i];
        } while (sMath[i] && isOperator(pChar) == FALSE);

        // NOTE: Concat immediatly because maybe this is last and pChar don't allow concat outside for loop
        strcat(result, pChar); strcat(result, ";");
      }

			// NOTE: Reset tmp
			for (iTmp=0; iTmp<MAX_STR; iTmp++) { tmp[iTmp] = '\0'; }
			iTmp=0; free(pChar);
		} else {
			// NOTE: This is just a number, we continue concat to create multi-number: 123
			tmp[iTmp++] = sMath[i];
		}
	}

	// NOTE: Append last element
	strcat(result, tmp);
	free(tmp);
	return result;
}

char* standardizeMath(char *sMath) {
  char **elementMath = split(sMath, ';');
  char *result = calloc(MAX_STR, sizeof (char));

  for (int i=0; elementMath[i]; i++) {
    if (ENV == DEVELOP) { printf("\n\n\t%d: %s", i, elementMath[i]); }

    // NOTE: Remove positive sign
    if (strcmp(elementMath[i], "+") == 0
        && (i == 0 || (i > 0 && isNumber(elementMath[i-1]) == FALSE && strcmp(elementMath[i-1], ")") != 0))
        && (elementMath[i+1] && (isNumber(elementMath[i+1]) == TRUE || strcmp(elementMath[i+1], "+") == 0))
    ) {
      continue;
    }

    // NOTE: Change negative sign to `~`
    if (strcmp(elementMath[i], "-") == 0
        && (i == 0 || (i > 0 && isNumber(elementMath[i-1]) == FALSE && strcmp(elementMath[i-1], ")") != 0))
        && (elementMath[i+1] && (isNumber(elementMath[i+1]) == TRUE || strcmp(elementMath[i+1], "-") == 0))
    ) {
      strcat(result, "~;");
      continue;
    }

    strcat(result, elementMath[i]);
    strcat(result, ";");
  }

  for (int i=0; elementMath[i]; i++) { free(elementMath[i]); }
  free(elementMath);
  return result;
}

int isOneMath(char *ope) {
  char operators[9][7] = {"sin", "cos", "tan", "arcsin", "arccos", "arctan", "sqrt", "log", "~"};
  for (int i=0; i<9; i++) {
    if (strcmp(ope, operators[i]) == 0) { return TRUE; }
  }
  return FALSE;
}

char* postfix(char *sMath) {
  char **elementMath = split(sMath, ';');
  char *result = calloc(MAX_STR, sizeof (char));
  char S[MAX_ELE][MAX_LEN_OPE];   // NOTE: Stack
  int iStack = 0;

  // NOTE: Initial Stack
  for (int i=0; i<MAX_ELE; i++) {
    for (int j=0; j<MAX_LEN_OPE; j++) {
      S[i][j] = '\0';
    }
  }

  for (int i=0; elementMath[i]; i++) {
    // NOTE: Operator is +, -, *, /, sin, cos,..
    if (isOperator(elementMath[i]) == TRUE) {
      if (strcmp(elementMath[i], "(") == 0) { strcpy(S[iStack++], "("); }
      else if (strcmp(elementMath[i], ")") == 0) {
        while (strcmp(S[iStack-1], "(") != 0) {
          strcat(result, S[iStack-1]); strcat(result, ";");
          for (int j=0; j<MAX_LEN_OPE; j++) { S[iStack-1][j] = '\0'; }
          iStack--;
        }

        // NOTE: Remove `(` from Stack
        for (int j=0; j<MAX_LEN_OPE; j++) { S[iStack-1][j] = '\0'; }
        iStack--;
      } else {
        // NOTE: elementMath[i] is operator and must has Priority(Stack.peek() >= elementMath[i])
        while (iStack > 0 && (getPriority(S[iStack-1]) >= getPriority(elementMath[i])) && isOneMath(elementMath[i]) == FALSE) {
          strcat(result, S[iStack-1]); strcat(result, ";");
          for (int j=0; j<MAX_LEN_OPE; j++) { S[iStack-1][j] = '\0'; }
          iStack--;
        }
        strcpy(S[iStack++], elementMath[i]);
      }
    } else {
      // NOTE: This is just a normal number
      strcat(result, elementMath[i]); strcat(result, ";");
    }
  }

  // NOTE: Pop all remain elements in Stack
  while (--iStack >= 0) { strcat(result, S[iStack]); strcat(result, ";"); }

  for (int i=0; elementMath[i]; i++) { free(elementMath[i]); }
  free(elementMath);
  return result;
}

double valueMath(char *sMath) {
  char **elementMath = split(sMath, ';');
  char *result = calloc(MAX_STR, sizeof (char));
  char S[MAX_ELE][MAX_LEN_OPE];   // NOTE: Stack
  int iStack = 0;

  // NOTE: Initial Stack
  for (int i=0; i<MAX_ELE; i++) {
    for (int j=0; j<MAX_LEN_OPE; j++) {
      S[i][j] = '\0';
    }
  }

  for (int i=0; elementMath[i]; i++) {
    // NOTE: Is number
    if (isOperator(elementMath[i]) == FALSE) { strcpy(S[iStack++], elementMath[i]); }
    else {
      double num;
      double num1 = atof(S[iStack-1]);
      for (int j=0; j<MAX_LEN_OPE; j++) { S[iStack-1][j] = '\0'; }
      iStack--;

      if (strcmp(elementMath[i], "sin") == 0) { num = sin(num1*PI/180); }
      else if (strcmp(elementMath[i], "cos") == 0) { num = cos(num1*PI/180); }
      else if (strcmp(elementMath[i], "~") == 0) { num = -num1; }
      else {
        double num2 = atof(S[iStack-1]);
        for (int j=0; j<MAX_LEN_OPE; j++) { S[iStack-1][j] = '\0'; }
        iStack--;

        if (strcmp(elementMath[i], "+") == 0) { num = num2 + num1; }
        else if (strcmp(elementMath[i], "-") == 0) { num = num2 - num1; }
        else if (strcmp(elementMath[i], "*") == 0) { num = num2 * num1; }
        else if (strcmp(elementMath[i], "/") == 0) { num = num2 / num1; }
      }

        // NOTE: Double to String
        snprintf(S[iStack++], MAX_LEN_OPE, "%f", num);
    }
  }

  return atof(S[--iStack]);;
}

double calculator(char *math) {
	char *sMath = standardize(math);
	char *pMath = processInput(sMath);
	char *smMath = standardizeMath(pMath);
	char *postfixMath = postfix(smMath);
	double result = valueMath(postfixMath);
	if (ENV == DEVELOP) {
    printf("\n\n\t>> standardize: %s\n", sMath);
    printf("\n\n\t>> processInput: %s\n", pMath);
    printf("\n\n\t>> standardizeMath: %s\n", smMath);
    printf("\n\n\t>> postfix: %s\n", postfixMath);
    printf("\n\n\t>> valueMath: %f\n", result);
	}

	free(sMath);
	free(pMath);
	free(smMath);
	free(postfixMath);

	return result;
}

#endif // BALAN_H_INCLUDED
