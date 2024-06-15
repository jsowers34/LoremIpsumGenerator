/*****************************************************************************/
/* BEGIN HEADER FILE     loremipsum.h                                        */
/*****************************************************************************/
/* PURPOSE                                                                   */
/*                                                                           */
/*****************************************************************************/
/* DESIGN NOTES                                                              */
/*     Based on Java code by Oliver C. Dodd                                  */
/*****************************************************************************/
/* REVISION HISTORY                                                          */
/*   Initial code:        JL Sowers    09JUN24                               */
/*   Modifications:                                                          */
/*                                                                           */
/*****************************************************************************/
/*  SAFETY DEFINITION                                                        */
/*							                                                 */

#ifndef   _LOREMIPSUM_H_
#define   _LOREMIPSUM_H_

/*                                                                           */
/*****************************************************************************/
/*  INCLUDE FILES                                                            */
/*                                                                          */

#pragma once

#include <QtWidgets/QMainWindow>
#include <QMenu>
#include <string>
#include "ui_loremipsum.h"
#include "Random.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoremIpsumClass; };
QT_END_NAMESPACE


class LoremIpsum : public QMainWindow {
    Q_OBJECT

public:
    LoremIpsum(QWidget *parent = nullptr);
    ~LoremIpsum();



	std::string randomWord ();
	std::string randomPunctuation ();
	std::string wordString (int count);
	std::string sentenceFragment (int mincount, int maxcount);
	std::string sentenceFragment ();
	std::string sentence ();
	std::string sentences (int count);
	std::string paragraph (bool useStandard, int minsent, int maxsent);
	std::string paragraph (bool useStandard);
	std::string paragraph ();
	std::string paragraphs (int count, bool useStandard);
	std::string paragraphs (int count);



private:
    Ui::LoremIpsumClass *ui;
	Random random;

	bool use_standard_sentence_f = false;

	int minWordCount = 3;
	int maxWordCount = 17;
	int minSentenceCount = 1;
	int maxSentenceCount = 5;
	const int minParagraphCount = 1;
	int maxParagraphCount = 1;

	void createMenus ();
	void createActions ();

	QMenu* fileMenu;
	QMenu* helpMenu;
	QAction* exitAct;
	QAction* aboutAct;
	QAction* helpAct;



    std::string standard = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.  Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	std::string words[185]{ "a", "ac", "accumsan", "ad", "adipiscing", "aenean", "aliquam", "aliquet", "amet", "ante", "aptent",
						    "arcu", "at", "auctor", "augue", "bibendum", "blandit", "class", "commodo", "condimentum", "congue",
						    "consectetur", "consequat", "conubia", "convallis", "cras", "cubilia", "cum", "curabitur", "curae", "cursus",
							"dapibus", "diam", "dictum", "dictumst", "dignissim", "dis", "dolor", "donec", "dui", "duis",
							"egestas", "eget", "eleifend", "elementum", "elit", "enim", "erat", "eros", "est", "et",
							"etiam", "eu", "euismod", "facilisi", "facilisis", "fames", "faucibus", "felis", "fermentum", "feugiat",
							"fringilla", "fusce", "gravida", "habitant", "habitasse", "hac", "hendrerit", "himenaeos", "iaculis", "id",
							"imperdiet", "in", "inceptos", "integer", "interdum", "ipsum", "justo", "lacinia", "lacus", "laoreet",
							"lectus", "leo", "libero", "ligula", "litora", "lobortis", "lorem", "luctus", "maecenas", "magna",
							"magnis", "malesuada", "massa", "mattis", "mauris", "metus", "mi", "molestie", "mollis", "montes",
							"morbi", "mus", "nam", "nascetur", "natoque", "nec", "neque", "netus", "nibh", "nisi",
							"nisl", "non", "nostra", "nulla", "nullam", "nunc", "odio", "orci", "ornare", "parturient",
							"pellentesque", "penatibus",	"per", "pharetra", "phasellus", "placerat", "platea", "porta", "porttitor", "posuere",
							"potenti", "praesent", "pretium", "primis", "proin", "pulvinar", "purus", "quam", "quis", "quisque",
							"rhoncus", "ridiculus", "risus", "rutrum", "sagittis", "sapien", "scelerisque", "sed", "sem", "semper",
							"senectus", "sit", "sociis", "sociosqu", "sodales", "sollicitudin", "suscipit", "suspendisse", "taciti", "tellus",
							"tempor", "tempus", "tincidunt", "torquent", "tortor", "tristique", "turpis", "ullamcorper", "ultrices", "ultricies",
							"urna", "ut", "varius", "vehicula", "vel", "velit", "venenatis", "vestibulum", "vitae", "vivamus",
							"viverra", "volutpat", "vulputate", "\0" };

	std::string *punctuation = new std::string[2] { ".", "?" };
	std::string line_separator = "\n";

	size_t NumberOfWords = sizeof (words) / sizeof (words[0]);

public slots:
	void processStandardSentence ();
	void processMinWordCount ();
	void processMaxWordCount ();
	void processMinSentenceCount ();
	void processMaxSentenceCount ();
	void processMaxParagraphCount ();
	void clear ();
	void generate ();
	void copy ();
	void about ();
	void help ();

};


#endif
/*                                                                           */
/* END HEADER FILE    loremipsum.h                                           */
/*****************************************************************************/