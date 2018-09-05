// Cmd_parse.cpp : Defines the entry point for the console application.
//
// NOTE:
// _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "string.h"
#include <stdio.h>


#define OP_END_SYMB			0
#define OP_END_NULL			1
#define OP_NULL_STR			2

#define MODE_CMD_NAME		0
#define MODE_CMD_PAR		1
#define MODE_CMD_VAL		2
#define MODE_CMD_PVAL		3

#define MODE_CMD_SEP_PAR	0
#define MODE_CMD_SEP_VAL	1

#define MAX_STR_TAG			8


#define CMD_COUNT			2


struct stCommandLine
{
	// description
	const char * name;
	const char * help;

	// parameters
	BYTE ucPars_count;
	const char * v_pars[6];

	// function
//	void(*test)(void);

};

static const stCommandLine mv_cmdLines[2] =
{
	{
		"cmd00",
		"cmd0_help",
		6,
		{ "abc00", "def01", "02", "03", "04", "05" },
//		cmd00
	},

	{
		"cmd01",
		"cmd1_help",
		6,
		{ "abc10", "def11", "12", "13", "14", "15" },
//		cmd01
	}

};

void cmd00(void)
{


};

void cmd01(void)
{


};



// INFO:
// function: proc line
// clauses:
// # mode 0 = parse cmd name
// uses separate symbols: '-', '=', '\0'
// # mode 1 = parse cmp param
// uses separate symbols: '=', '\0'
// # mode 2 = parse cmp value
// uses separate symbols: '\0'
BYTE GetStrSep(char * strParse, char * strOutput, BYTE * ucSepNum, BYTE mode)
{
	const BYTE const_MAX_SEP_CHARS = 8;

	struct 
	{
		BYTE ucCount;
		char v_char[const_MAX_SEP_CHARS];

	} v_mode[3] = {
		// mode 0:
		{
			2,
			{ '-', '=' }
		},
		// mode 1:
		{
			1,
			{ '=' }
		},
		// mode 2:
		{
			0,
			{  }
		}
	};

	
	// NOTE:
	// cycle proc in next modes:
	// act = 0: cycle proc end
	// act = 1: try find Separate symbol op
	char strTag[MAX_STR_TAG];

	BYTE k = 0;
	BYTE ucEndPos = 0;
	char c;

	BYTE act = 1;
	while (act != 0)
	{
		// get iteration Symbol
		c = strParse[k];

		// check EOL
		if (c == '\0')
		{
			// [EOS]

			// define a Reason
			if (k == 0)
			{
				// [EMPTY LINE]

				return OP_NULL_STR;
			}
			else 
			{
				// [NO SEPARATE SYMBOL FOUND]

				return OP_END_NULL;
			}
		}// > Check End of Cycle /EOS

		// > Proceed String
		else 
		{
			// [proc find Separate symbol]

			BYTE ucSepNumber = 0;
			while (ucSepNumber < v_mode[mode].ucCount)
			{
				// check separate coincidence
				if (c == v_mode[mode].v_char[ucSepNumber])
				{
					// [COINCIDE]

					// store coincide separate symbol
					*ucSepNum = ucSepNumber;

					// define End Pos
					ucEndPos = k;

					// trigger flag
					act = 0;

					// activate stop search op
					ucSepNumber = v_mode[mode].ucCount;
				}
				
				ucSepNumber++;

			}//while ucSepNum

			// next symbol
			k++;
		}

	}//while (act != 0)

	// > Form Output String /tag/
	for (BYTE kk = 0; kk < ucEndPos; kk++)
	{
		strOutput[kk] = strParse[kk];
	}

	strOutput[ucEndPos] = '\0';


	return OP_END_SYMB;
}


BYTE Proc_CommandLine(char * strInput)
{
	BYTE ucOpState;
	BYTE ucSepType;
	BYTE ucOpMode;

	char strBuf[64];
	char strCmdName[16];
	char strCmdPar[16];
	char strCmdVal[32];
	

	// > Stage 1: Get Cmd Config
	// > Get Name
	ucOpState = GetStrSep(strInput, strBuf, &ucSepType, MODE_CMD_NAME);

	if (ucOpState == OP_NULL_STR)
	{
		// [EMPTY CMD]

		return 1;
	}
	else if (ucOpState == OP_END_NULL)
	{
		// [CMD WITH NO VALUE]

		// Set Name
		strcpy(strCmdName, strInput);
		// Set Mode
		ucOpMode = MODE_CMD_NAME;
	}
	else if (ucOpState == OP_END_SYMB)
	{
		// [CMD HAS SOMETHING]

		// Set Name
		strcpy(strCmdName, strBuf);

		// Shift CmdLine
		BYTE ucPos = strlen(strBuf);
		strcpy(strBuf, strInput + ucPos + 1);

		if (ucSepType == MODE_CMD_SEP_VAL)
		{
			// [CMD WITH VALUE]
		
			// Set Value
			ucOpState = GetStrSep(strBuf, strCmdVal, &ucSepType, MODE_CMD_VAL);

			// check NULL
			if (ucOpState != OP_NULL_STR)
			{
				// [NOT NULL]

				// Set Value
				strcpy(strCmdVal, strBuf);

				// Set Mode
				ucOpMode = MODE_CMD_VAL;
			}
			else
			{
				// [EMPTY CMD]

				return 1;
			}
		}
		else if (ucSepType == MODE_CMD_SEP_PAR)
		{
			// [CMD HAS PAR]

			// Set Parameter
			ucOpState = GetStrSep(strBuf, strCmdPar, &ucSepType, MODE_CMD_PAR);

			// check NULL
			if (ucOpState != OP_NULL_STR)
			{
				// [NOT NULL]

				// check Par only clause
				if (ucOpState == OP_END_NULL)
				{
					// [CMD WITH PAR ONLY]

					// Set Param
					strcpy(strCmdPar, strBuf);

					// Set Mode
					ucOpMode = MODE_CMD_PAR;
				}
				else
				{
					// [CMD WITH PAR AND VALUE]

					// Shift CmdLine
					BYTE ucPos = strlen(strCmdPar);
					strcpy(strBuf, strBuf + ucPos + 1);

					// Set Value
					ucOpState = GetStrSep(strBuf, strCmdVal, &ucSepType, MODE_CMD_VAL);

					// check NULL
					if (ucOpState != OP_NULL_STR)
					{
						// [NOT NULL]

						// Set Value
						strcpy(strCmdVal, strBuf);

						// Set Mode
						ucOpMode = MODE_CMD_PVAL;
					}
					else
					{
						// [EMPTY FRAGMENT]

						return 1;
					}
				}//check Par only clause
			}
			else
			{
				// [EMPTY FRAGMENT]

				return 1;
			}
		}//[CMD HAS PAR]
	}//[CMD HAS SOMETHING]

	//!debug
	printf("Nam = %s \n", strCmdName);
	printf("Par = %s \n", strCmdPar);
	printf("Val = %s \n", strCmdVal);


	// > Stage 2: Check Cmd Config
	BYTE ucCmdName;
	BYTE ucCmdPar;

	// check Cmd Valid Name
	BYTE bValidCmdLine = 0;
	for (BYTE k = 0; k < CMD_COUNT; k++)
	{
		if ((strcmp(strCmdName, mv_cmdLines[k].name)) == 0)
		{
			// set Cmd Config
			ucCmdName = k;
			bValidCmdLine = 1;

			break;
		}
	}

	if (bValidCmdLine)
	{
		// [VALID]

		// check existing parameters: PARAMETER
		if ((ucOpMode == MODE_CMD_PAR) || (ucOpMode == MODE_CMD_PVAL))
		{
			// [HAVE PAR]

			bValidCmdLine = 0;
			for (BYTE k = 0; k < CMD_COUNT; k++)
			{
				if ((strcmp(strCmdPar, mv_cmdLines[ucCmdName].v_pars[k])) == 0)
				{
					// set Cmd Config
					ucCmdPar = k;
					bValidCmdLine = 1;

					break;
				}
			}
		}
	}
	else
	{
		// [BAD NAME]

		return 2;
	}

	if (bValidCmdLine)
	{
		// [VALID]

		// check existing parameters: VALUE
		if ((ucOpMode == MODE_CMD_VAL) || (ucOpMode == MODE_CMD_PVAL))
		{
			// [HAVE VAL]

			// specific actions
			// .. 
			bValidCmdLine = 1;
		}
	}
	else
	{
		// [BAD PAR]

		return 2;
	}


	// > Stage 3: Execute
	if (bValidCmdLine)
	{
		// [VALID]
		// execute specific Command
		mv_cmdLines[ucCmdName].ucPars_count;

		printf("valid command. \n");
	}
	else
	{
		// [BAD VAL]

		return 2;
	}

	// success
	return 0;
}



// Main routine
//////////////////////////////////////////////////////////////////////
// Main routine
//////////////////////////////////////////////////////////////////////
int main(int argc, char * argv[])
{
	// > Output Common Info
	printf("Wellcome to the CommandParser! [ver 1.0] \n\n");

	// > Output Parameters
	printf("Used parameters:\n");
	printf("- executed from: %s\n", argv[0]);

	if (argc > 1)
	{
		// [PARAMS EXISTS]
		// output Values
		for (int k = 0; k < argc; k++)
		{
			printf("- parameter %d: %s\n", k, argv[k]);
		}
	}
	else
	{
		// [NO PARAMS]
		// output Status
		printf("[command line has no additional arguments]\n");
	}


	// > Proceed commands
	BYTE act = 1;
	while (act)
	{
		// Define OP Cycle
		printf("\n %s \n %s \n", "- Press 'C' to input command line.", "- Press 'X' to exit.");

		char c = getchar();
		if ((c == 'x') || (c == 'X'))
		{
			// [EXIT]			

			printf("Press any key to exit... ");
			getchar();
			getchar();

			// exit cmp proc
			act = 0;			

		}
		else if ((c == 'c') || (c == 'C'))
		{
			// [CMD CYCLE]

			// input command line OP
			printf("input command line: \n");
			getchar();

			char strCmdLine[64];
			gets_s(strCmdLine);
			//printf("%s \n", strCmdLine);

			// parse command
			Proc_CommandLine(strCmdLine);

			//fget(strCmdLine[64]);
		}
	}	


	return 0;
}



