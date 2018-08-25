// Cmd_parse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string.h"


#define OP_END_SYMB			0
#define OP_END_NULL			1
#define OP_NULL_STR			2

#define MAX_STR_TAG			8

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
			}//while ucSepNum

			// next symbol
			k++;
		}

	}//while (act != 0)

	// > Form Output String /tag/
	for (BYTE kk = 0; kk <= ucEndPos; kk++)
	{
		strOutput[kk] = strParse[kk];
	}

	strOutput[ucEndPos + 1] = '\0';


	return OP_END_SYMB;
}


// Main routine
int main()
{

	const BYTE cucCMD_COUNT = 20;

	typedef const char * cc;
	typedef cc v1_cc[cucCMD_COUNT];
	typedef v1_cc v2_cc[];

	typedef cc cc_vector[];

	struct stCommandLine
	{
		// description
		const char * name;
		const char * help;

		// parameters
		BYTE ucPars_count;
		cc v_pars[6];

		// function
		//void *test(void);

	};

	static const stCommandLine v_cmdLines[2] =
	{
		{
			"cmd00",
			"cmd0_help",
			6,
			{ "00", "01", "02", "03", "04", "05" },
		},

		{
			"cmd01",
			"cmd1_help",
			6,
			{ "10", "11", "12", "13", "14", "15" },
		}

	};


	v2_cc v_values =
	{
		{ "00", "01", "02", "03", "04", "05" },
		{ "10", "11", "12", "13", "14", "15" },
		{ "20", "21", "22", "23", "24", "25" },
		{ "30", "31", "32", "33", "34", "35" },
		{ "40" },
		{ "50" }
	};


	const char * str1 = v_values[5][0];

	//Trace(_T("start\n"));
	//Trace(_T("%c \n"), str1);


    return 0;
}


