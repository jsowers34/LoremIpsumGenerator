/****************************************************************************/
/* BEGIN FILE             loremipsum.cpp                                    */
/****************************************************************************/
/* PURPOSE                                                                  */
/*                                                                          */
/****************************************************************************/
/* DESIGN NOTES                                                             */
/*        Some code based on Java code by Oliver Dodd                       */
/****************************************************************************/
/* REVISION HISTORY                                                         */
/*                                                                          */
/*   Initial Code:                                                          */
/*      @author    jsowers             09JUN24                              */
/*                                                                          */
/*   Modifications:                                                         */
/*                                                                          */
/****************************************************************************/
/* INCLUDE FILES                                                            */
/*                                                                          */

#include <QMenuBar>
#include <QClipBoard>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "loremipsum.h"
#include "AboutDialog.h"
#include "HelpDialog.h"

/*                                                                          */
/****************************************************************************/
/* DEFINES/NAMESPACES                                                       */
/*                                                                          */ 

using namespace std;

/****************************************************************************/

/**
* Default Constructor.
*/
LoremIpsum::LoremIpsum(QWidget *parent) : QMainWindow(parent), ui(new Ui::LoremIpsumClass()) {
    ui->setupUi(this);
    random.reset (-1);
    ui->copyBtn->setDisabled (true);   // No copy without text present

    connect (ui->stdCB, &QCheckBox::checkStateChanged, this, &LoremIpsum::processStandardSentence);
    connect (ui->minLE, &QLineEdit::textChanged, this, &LoremIpsum::processMinWordCount);
    connect (ui->maxLE, &QLineEdit::textChanged, this, &LoremIpsum::processMaxWordCount);
    connect (ui->minSentLE, &QLineEdit::textChanged, this, &LoremIpsum::processMinSentenceCount);
    connect (ui->maxSentLE, &QLineEdit::textChanged, this, &LoremIpsum::processMaxSentenceCount);
    connect (ui->numParaLE, &QLineEdit::textChanged, this, &LoremIpsum::processMaxParagraphCount);

    connect (ui->clearBtn, &QPushButton::clicked, this, &LoremIpsum::clear);
    connect (ui->genBtn, &QPushButton::clicked, this, &LoremIpsum::generate);
    connect (ui->copyBtn, &QPushButton::clicked, this, &LoremIpsum::copy);

    createActions ();
    createMenus ();
}

/**
* Default Destructor.
*/
LoremIpsum::~LoremIpsum() {
    delete ui;
}

/**
 * Build all the menu choices relating the menu item to their action.
 */
void LoremIpsum::createMenus () {
    fileMenu = menuBar ()->addMenu (tr ("&File"));
    fileMenu->addAction (exitAct);

    helpMenu = menuBar ()->addMenu (tr ("&Help"));
    helpMenu->addAction (aboutAct);
    helpMenu->addAction (helpAct);
}

/**
 * Localized method to create all of the Actions needed.
 */
void LoremIpsum::createActions () {
    exitAct = new QAction (tr ("E&xit"), this);
    exitAct->setShortcuts (QKeySequence::Quit);
    exitAct->setStatusTip (tr ("Exit the application"));
    connect (exitAct, &QAction::triggered, this, &QWidget::close);


    aboutAct = new QAction (tr ("&About"), this);
    aboutAct->setStatusTip (tr ("Show the application's About box"));
    connect (aboutAct, &QAction::triggered, this, &LoremIpsum::about);

    helpAct = new QAction (tr ("&Help"), this);
    helpAct->setStatusTip (tr ("Show the application's Help box"));
    connect (helpAct, &QAction::triggered, this, &LoremIpsum::help);
}

/**
 * Display the brag box.
 * @brief MainWindow::about
 */
void LoremIpsum::about () {
    AboutDialog* about = new AboutDialog (this);
    about->exec ();
}

/**
 * Display a rudimentary help file.
 * @brief MainWindow::help
 */
void LoremIpsum::help () {
    HelpDialog* help = new HelpDialog (this);
    help->exec ();
}

/**
* Callback method for the 'Standard' paragraph Checkbox.
*/
void LoremIpsum::processStandardSentence () {
    use_standard_sentence_f = ui->stdCB->isChecked ();
    ui->resultTB->clear ();
    if (use_standard_sentence_f) {
        ui->minLE->setDisabled (true);
        ui->maxLE->setDisabled (true);
    } else {
        ui->minLE->setDisabled (false);
        ui->maxLE->setDisabled (false);
    }
}

/**
* Callback method used to capture the minimum work count allowed.
*/
void LoremIpsum::processMinWordCount () {
    minWordCount = stoi(ui->minLE->text ().toStdString());
}

/**
* Callback method used to capture the maximum work count allowed.
*/
void LoremIpsum::processMaxWordCount () {
    maxWordCount = stoi (ui->maxLE->text ().toStdString ());
}

/**
* Callback method used to capture the minimum senetence count allowed.
*/
void LoremIpsum::processMinSentenceCount () {
    minSentenceCount = stoi (ui->minSentLE->text ().toStdString ());
}

/**
* Callback method used to capture the maximum senetence count allowed.
*/
void LoremIpsum::processMaxSentenceCount () {
    maxSentenceCount = stoi (ui->maxSentLE->text ().toStdString ());
}

/**
* Callback method used to capture the maximum paragraph count allowed.
*/
void LoremIpsum::processMaxParagraphCount () {
    maxParagraphCount = stoi (ui->numParaLE->text ().toStdString ());
}

/**
*   Clear and reset the screen.
*/
void LoremIpsum::clear () {
    ui->minLE->setText (tr("3"));
    ui->maxLE->setText (tr("10"));
    ui->numParaLE->setText (tr ("1"));
    ui->minSentLE->setText (tr ("1"));
    use_standard_sentence_f = false;
    ui->stdCB->setChecked (use_standard_sentence_f);
    ui->resultTB->clear ();
    ui->copyBtn->setDisabled (true);
}

/**
* Generate the text according to the selected fields.
*/
void LoremIpsum::generate () {
    ui->resultTB->setText (QString::fromStdString(paragraphs (maxParagraphCount, use_standard_sentence_f)));
    ui->copyBtn->setDisabled (false);
}

/**
* Copy the generated text to the clipboard so that it may be pasted into another application, etc.
*/
void LoremIpsum::copy () {
    QClipboard* clipboard = QApplication::clipboard ();
    clipboard->setText (ui->resultTB->toPlainText());
}

/**
* Get a random word.
*/
string LoremIpsum::randomWord () {
    int rword = random.uniform (1, 0, NumberOfWords - 1);
	return words[rword];
}

/**
* Get a random punctuation mark.
*/
string LoremIpsum::randomPunctuation () {
    int randInt = random.uniform (1, 0, 1);
    return punctuation[randInt];
}

/**
* Get a string of words.
* @param count is the number of words to fetch.
*/
string LoremIpsum::wordString (int count) {
    string s;
    while (count-- > 0)
        s.append (randomWord ()).append (" ");
    return s;
}

/**
 * Get a sentence fragment.
 * @param mincount  the smallest number of possible words.
 * @param maxcount  the largest number of possible words.
 */
string LoremIpsum::sentenceFragment (int mincount, int maxcount) {
    int randInt = mincount + random.uniform (1, 0, maxcount-mincount);
    return wordString(randInt);
}

/**
 * Get a sentence fragment use the input fields to determine the parameters.
 */
string LoremIpsum::sentenceFragment () {
    return sentenceFragment (minWordCount, maxWordCount);
}

/**
 * Get a sentence.
 */
string LoremIpsum::sentence () {
    // first word
    string w = randomWord ();
    string w0 = w.substr (0, 1);
    boost::to_upper (w0);
    string s = w0.append (w.substr (1, w.length () - 1)).append (" ");

    // commas?
    if (random.random_boolean(1)) {
        int r = 1 + random.uniform (1, 0, 3);
        for (int i = 0; i < r; i++)
            s.append (sentenceFragment ()).append (", ");
    }
    // last fragment + punctuation
    return s.append (sentenceFragment ()).append (randomPunctuation ());
}

/**
* Get multiple sentences.
* @param count - the number of sentences
*/
string LoremIpsum::sentences (int count) {
    string s = "";
    while (count-- > 0)
        s.append (sentence ()).append ("  ");
    return s;
}

/**
 * Get a paragraph.
 * @useStandard - get the standard Lorem Ipsum paragraph?
 * @param minsent the minimum number of sentences
 * @param maxsent the maximum number of sentences
 */
string LoremIpsum::paragraph (bool useStandard, int minsent, int maxsent) {
    int rval = minsent + random.uniform (1, 0, maxsent-minsent);
    return useStandard ? standard : sentences (rval);
}

/**
 * Get a paragraph.
 * Use the input fields to determine the parameters.
 * @useStandard - get the standard Lorem Ipsum paragraph?
 */
string LoremIpsum::paragraph (bool useStandard) {
    return paragraph (useStandard, minSentenceCount, maxSentenceCount);
}


/**
 * Get a standard paragraph.
 * Get the standard Lorem Ipsum paragraph.
 */
string LoremIpsum::paragraph () {
    return paragraph (false, 1, 1);
}

/**
 * Get multiple paragraphs.
 * @param count is the number of paragraphs
 * @useStandard - begin with the standard Lorem Ipsum paragraph if true.
 */
string LoremIpsum::paragraphs (int count, bool useStandard) {
    string s = "";
    while (count-- > 0) {
        s.append (paragraph (useStandard)).append (line_separator).append (line_separator);
        use_standard_sentence_f = false;
        useStandard = false;  // Only the 1st paragraph is standard
    }
    return s;
}

/**
 * Get multiple paragraphs.
 */
string LoremIpsum::paragraphs (int count) {
    return paragraphs (count, false);
}


