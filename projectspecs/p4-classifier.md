---
layout: spec
latex: true
mermaid: true
---

EECS 280 Project 4: Machine Learning
====================================
{: .primer-spec-toc-ignore }

Winter 2025 release.

Project due 8:00pm EST Friday March 28, 2024.

You may work alone or with a partner ([partnership guidelines](https://eecs280.org/syllabus.html#project-partnerships)).

## Introduction

Automatically identify the subject of posts from the EECS 280 Piazza using natural language processing and machine learning techniques.

The learning goals of this project include using container ADTs, such as sets and maps. You will also gain experience designing and implementing a substantial application program.

For example, your program will be able to read a Piazza post like this and figure out that it's about Project 3: Euchre.

<img src="images/image28.png" width="640px" />


## Setup
Set up your visual debugger and version control, then submit to the autograder.

### Visual debugger
During setup, name your project `ml-classifier`. Use this starter files link: `https://eecs280staff.github.io/ml-classifier/starter-files.tar.gz`

| [VS Code](https://eecs280staff.github.io/tutorials/setup_vscode.html)| [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html) | [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html) |

You should end up with a folder with starter files that look like this. You may also have a `main.cpp` file after following the setup tutorial, which you should rename to `classifier.cpp`. If not, you will create a `classifier.cpp` file in the [Classifier](#classifier) section.
```console
$ ls
Makefile						train_small.csv
csvstream.hpp					train_small_train_only.out.correct
instructor_student.out.correct	w14-f15_instructor_student.csv
projects_exam.out.correct		w16_instructor_student.csv
sp16_projects_exam.csv			w16_projects_exam.csv
test_small.csv					w16_projects_exam_train_only.out.correct
test_small.out.correct
```
{: data-variant="no-line-numbers" }

Here's a short description of each starter file.

| File(s) | Description |
| ------- | ----------- |
| `csvstream.hpp` | Library for reading CSV files. |
| `train_small.csv`<br> `test_small.csv`<br> `test_small.out.correct`<br> `train_small_train_only.out.correct` | Sample input and output for the classifier. |
| `sp16_projects_exam.csv`<br> `w14-f15_instructor_student.csv`<br> `w16_instructor_student.csv`<br> `w16_projects_exam.csv`<br>`w16_projects_exam_train_only.out.correct`<br> `instructor_student.out.correct`<br> `projects_exam.out.correct` | Piazza data input from past terms, with correct output. |
| `Makefile` | Helper commands for building. |

### Version control
Set up version control using the [Version control tutorial](https://eecs280staff.github.io/tutorials/setup_git.html).

After you're done, you should have a local repository with a "clean" status and your local repository should be connected to a remote GitHub repository.
```console
$ git status
On branch main
Your branch is up-to-date with 'origin/main'.

nothing to commit, working tree clean
$ git remote -v
origin	https://github.com/awdeorio/ml-classifier.git (fetch)
origin	https://githubcom/awdeorio/ml-classifier.git (push)
```

You should have a `.gitignore` file ([instructions](https://eecs280staff.github.io/tutorials/setup_git.html#add-a-gitignore-file)).
```console
$ head .gitignore
# This is a sample .gitignore file that's useful for C++ projects.
...
```

### Group registration
Register your partnership (or working alone) on the  [Autograder](https://autograder.io/).  Then, submit the code you have.


## ML and NLP Background
<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Skim this section the first time through.  Refer back to it while you're coding the [Classifier](#classifier).
</div>

### Machine Learning and Classification

The goal for this project is to write an intelligent program that can
**classify** Piazza posts according to topic. This task is easy for humans -
we simply read and understand the content of the post, and the topic is
intuitively clear. But how do we compose an algorithm to do the same? We
can't just tell the computer to "look at it" and understand. This is
typical of problems in artificial intelligence and natural language
processing.

![](images/image28.png)

We know this is about Euchre, but how can we write an algorithm that
"knows" that?

With a bit of introspection, we might realize each individual word is a
bit of evidence for the topic about which the post was written. Seeing a
word like "card" or "spades" leads us toward the Euchre
project. We judge a potential label for a post based on how likely it is
given all the evidence. Along these lines, information about how common
each word is for each topic essentially constitutes our classification
algorithm.

But we don't have that information (i.e. that algorithm). You could try
to sit down and write out a list of common words for each project, but
there's no way you'll get them all. For example, the word "lecture"
appears much more frequently in posts about exam preparation. This makes
sense, but we probably wouldn't come up with it on our own. And what if
the projects change? We don't want to have to put in all that work
again.

Instead, let's write a program to comb through Piazza posts from
previous terms (which are already tagged according to topic) and learn
which words go with which topics. Essentially, the result of our program
is an algorithm! This approach is called (supervised) machine learning.
Once we've trained the classifier on some set of Piazza posts, we can
apply it to new ones written in the future.

![](images/image29.png){: .invert-colors-in-dark-mode }

At a high level, the classifier we'll implement works by assuming a
probabilistic model of how Piazza posts are composed, and then finding
which label (e.g. our categories of  "euchre", "exam", etc.) is the most
probable source of a particular post.

All the details of natural language processing (NLP) and machine
learning (ML) techniques you need to implement the project are described
here. You are welcome to consult other resources, but there are many
kinds of classifiers that have subtle differences. The classifier we
describe here is a simplified version of a "Multi-Variate Bernoulli
Naive Bayes Classifier". If you find other resources, but you're not
sure they apply, make sure to check them against this specification.

[This document](naive_bayes.html) provides a more complete description
of the way the classifier works, in case you're interested in the math
behind the formulas here.

### Piazza Dataset

For this project, we retrieved archived Piazza posts from EECS 280 in
past terms. We will focus on two different ways to divide Piazza posts
into labels (i.e. categories).

- By **topic**. Labels: "exam", "calculator", "euchre", "image", "recursion",
  "statistics"

  Example: Posts extracted from `w16_projects_exam.csv`

  | label | content |
  | ----- | ------- |
  | exam | will final grades be posted within 72 hours |
  | calculator | can we use the friend class list in stack |
  | euchre | weird problem when i try to compile euchrecpp |
  | image | is it normal for the horses tests to take 10 minutes |
  | recursion | is an empty tree a sorted binary tree |
  | statistics | are we supposed to have a function for summary |
  | ... | ... |

- By **author**. Labels: "instructor", "student"

  Example: Posts extracted from `w14-f15_instructor_student.csv`

  | label | content |
  | ----- | ------- |
  | instructor | disclaimer not actually a party just extra OH |
  | student | how can you use valgrind with calccpp |
  | student | could someone explain to me what the this keyword means |
  | ... | ... |

The Piazza datasets are Comma Separated Value (CSV) files. The label for
each post is found in the "tag" column, and the content in the
"content" column. There may be other columns in the CSV file; your
code should ignore all but the "tag" and "content" columns. **You may
assume all Piazza files are formatted
correctly, and that post content and labels only contain lowercase
characters, numbers, and no punctuation.** You must use the
`csvstream.hpp` library (see
[https://github.com/awdeorio/csvstream](https://github.com/awdeorio/csvstream) for
documentation) to read CSV files in your application. The
`csvstream.hpp` file itself is included with the starter code.

**Your classifier should not hardcode any labels. Instead, it should use
the exact set of labels that appear in the training data.**

<div id="splitting-a-whitespace-delimited-string" class="primer-spec-callout info" markdown="1">
**Pro-tip:** Here's how to split a string into words. You may use this code as given.
```c++
// EFFECTS: Return a set of unique whitespace delimited words
set<string> unique_words(const string &str) {
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word) {
    words.insert(word);
  }
  return words;
}
```
</div>

We have included several Piazza datasets with the project:
  - `train_small.csv` - Made up training data intended for small-scale
    testing.
  - `test_small.csv` - Made up test data intended for small-scale
    testing.
  - `w16_projects_exam.csv` - (Train) Real posts from W16 labeled by
    topic.
  - `sp16_projects_exam.csv` - (Test) Real posts from Sp16 labeled by
    topic.
  - `w14-f15_instructor_student.csv` - (Train) Real posts from four
    terms labeled by author.
  - `w16_instructor_student.csv` - (Test) Real posts from W16 Piazza
    labeled by author.

For the real datasets, we have indicated which are intended for training
vs. testing.

### Bag of Words Model

We will treat a Piazza post as a "**bag of words**" - each post is simply characterized by which words it includes. The ordering of words is ignored, as are multiple occurrences of the same word. These two posts would be considered equivalent:
- "the left bower took the trick"
- "took took trick the left bower bower"

Thus, we could imagine the post-generation process as a person sitting down and going through every possible word and deciding which to toss into a bag.

#### Conditional Probability

We write $$ P(A) $$ to denote the probability (a number
between 0 and 1) that some event $$ A $$ will occur.
$$ P(A \mid B) $$ denotes the probability that event
$$ A $$ will occur given that we already know event
$$ B $$ has occurred. For example,
$$ P(bower \mid euchre) \approx 0.007 $$. This means that if a Piazza post is about the
euchre project, there is a 0.7% chance it will contain the word bower
(we should say "at least once", technically, because of the bag of words
model).

### Training

Before the classifier can make predictions, it needs to be trained on a
set of previously labeled Piazza posts (e.g. `train_small.csv` or
`w16_projects_exam.csv`). Your application should process each post in
the training set, and record the following information:

  - The total number of posts in the entire training set.
  - The number of unique words in the entire training set. (The
    **vocabulary size**.)
  - For each word $$ w $$, the number of posts in the
    entire training set that contain $$ w $$.
  - For each label $$ C $$, the number of posts with that
    label.
  - For each label $$ C $$ and word
    $$ w $$, the number of posts with label
    $$ C $$ that contain $$ w $$.

### Prediction

How do we predict a label for a new post?

Given a new Piazza post $$ X $$, we must determine the
most probable label $$ C $$, based on what the classifier
has learned from the training set. A measure of the likelihood of $$ C $$ is
the **log-probability score** given the post:

$$
\ln P(C) + \ln P(w_1 \mid C) + \ln P(w_2 \mid C) + \cdots + \ln P(w_n \mid C)
$$

**Important**: Because we're using the bag-of-words model, the words $$ w_1, w_2, \ldots, w_n $$ in this formula are only the [unique
words](#splitting-a-whitespace-delimited-string) in the
post, not including duplicates\! To ensure consistent results, make
sure to add the contributions from each word in alphabetic order.

The classifier should predict whichever label has the highest
log-probability score for the post. If multiple labels are tied, predict
whichever comes first alphabetically.

$$ \ln P(C) $$ is the **log-prior** probability of label
$$ C $$ and is a reflection of how common it is:

$$
\ln P(C) = \ln \left( \frac{\text{number of training posts with label } C}{\text{number of training posts}} \right)
$$

$$ \ln P(w \mid C) $$ is the **log-likelihood** of a word
$$ w $$ given a label $$ C $$, which is a
measure of how likely it is to see word $$ w $$ in posts
with label $$ C $$. The regular formula for
$$ \ln P(w \mid C) $$ is:

$$
\ln P(w \mid C) = \ln \left( \frac{\text{number of training posts with label } C \text{ that contain } w}{\text{number of training posts with label } C} \right)
$$

However, if $$ w $$ was never seen in a post with label
$$ C $$ in the training data, we get a log-likelihood of
$$ -\infty $$, which is no good. Instead, use one of these two alternate formulas:

---

$$
\ln P(w \mid C) = \ln \left( \frac{\text{number of training posts that contain } w}{\text{number of training posts}} \right)
$$

(Use when $$ w $$ does not occur in posts labeled $$ C $$ but does occur in the training data overall.)

---

$$
\ln P(w \mid C) = \ln \left( \frac{1}{\text{number of training posts}} \right)
$$

(Use when $$ w $$ does not occur anywhere at all in the
training set.)

---


## Classifier

Write the classifier in `classifier.cpp` using the [bag of words model](#bag-of-words-model).

Run the classifier on a small dataset.
```console
$ ./classifier.exe train_small.csv test_small.csv
```

### Setup

If you created a `main.cpp` while following the setup tutorial, rename it to `classifier.cpp` if you have not already done so.  Otherwise, create a new file `classifier.cpp` ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#add-new-files), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#add-new-files), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#add-new-files),  [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#add-new-files), [CLI](https://eecs280staff.github.io/tutorials/cli.html#touch)).

Add "hello world" code if you haven't already.
```c++
#include <iostream>
using namespace std;

int main() {
  cout << "Hello World!\n";
}
```

The classifier program should compile and run.
```console
$ make classifier.exe
$ ./classifier.exe
Hello World!
```

Configure your IDE to debug the classifier program.

<table>
<tr>
  <td>
  <b>VS Code (macOS)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/classifier.exe`
  </td>
</tr>
<tr>
  <td>
  <b>VS Code (Windows)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/classifier.exe`
  </td>
</tr>
<tr>
  <td>
  <b>Xcode</b>
  </td>
  <td markdown="1">
  Include [compile sources](https://eecs280staff.github.io/tutorials/setup_xcode.html#compile-sources): <br>
  `classifier.cpp`
  </td>
</tr>
<tr>
  <td>
  <b>Visual Studio</b>
  </td>
  <td markdown="1">
  [Exclude files](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#exclude-files-from-build) from the build: <br>
  - Include `classifier.cpp`
  - Exclude any other tests
  </td>
</tr>
</table>

Configure command line arguments ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#arguments-and-options), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#arguments-and-options), [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#arguments-and-options), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#arguments-and-options)). We recommend starting with the smallest input in train-only mode, `train_small.csv`.

To compile and run the smallest input at the command line:
```console
$ make classifier.exe
$ ./classifier.exe train_small.csv
```

### Command Line Interface

Here is the usage message for the top-level application:
```console
$ ./classifier.exe
Usage: classifier.exe TRAIN_FILE [TEST_FILE]
```

The classifier application always requires a file for training, and it optionally takes a file for testing. The training file must have at least one post, but the test file may have no posts. You may assume all files are in the correct format, with a header that has at least the "tag" and "content" columns.

Use the provided small-scale files for initial testing and to check your output formatting:

```console
$ ./classifier.exe train_small.csv
$ ./classifier.exe train_small.csv test_small.csv
```

Correct output is in `train_small_train_only.out.correct` and
`test_small.out.correct`. The output format is discussed in detail below.

#### Error Checking

The program checks that the command line arguments obey the following
rule:

- There are 2 or 3 arguments, including the executable name itself
  (i.e. `argv[0]`).

If this is violated, print out the usage message and then quit
by returning a non-zero value from `main`. **Do not use the `exit`
library function, as this fails to clean up local objects.**

```c++
cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
```
{: data-variant="no-line-numbers" }

If any file cannot be opened, print out the following message, where
`filename` is the name of the file that could not be opened, and quit by
returning a non-zero value from `main`.

```c++
cout << "Error opening file: " << filename << endl;
```
{: data-variant="no-line-numbers" }

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** The `csvstream` constructor will throw a `csvstream_exception` containing the correct error message if a file cannot be opened. The example at [https://github.com/awdeorio/csvstream#error-handling](https://github.com/awdeorio/csvstream#error-handling) shows how to handle the exception with a `try`/`catch` block.
</div>

You do not need to do any error checking for command-line arguments or
file I/O other than what is described on this page. However, you must
use precisely the error messages given here in order to receive credit.
(**Just literally use the code given here to print them.**)

As mentioned earlier, you may assume all Piazza data files are in the
correct format.

### Design

Here is some high-level guidance:

1.  First, your application should read posts from a file (e.g.
    `train_small.csv`) and use them to train the classifier. After
    training, your classifier abstraction should store the information
    mentioned in the [Training](#training) section.
2.  Your classifier should be able to compute the log-probability
    score of a post (i.e. a collection of words) given a particular
    label. To predict a label for a new post, it should choose the label
    that gives the highest log-probability score.  See the [Prediction](#prediction) section.
3.  Read posts from a file (e.g. `test_small.csv`) to use as testing
    data. For each post, predict a label using your classifier.

Some of these steps have output associated with them. See the "output"
section below for the details.

The structure
of your classifier application, including which procedural abstractions
and/or ADTs to use for the classifier, is entirely up to you. Make sure
your decisions are informed by carefully considering the classifier and
top-level application described in this specification.

We **strongly** suggest you make a class to represent the classifier - the
private data members for the class should keep track of the classifier
parameters learned from the training data, and the public member
functions should provide an interface that allows you to train the
classifier and make predictions for new piazza posts.

You should write RMEs and appropriate comments to describe the
interfaces for the abstractions you choose (ADTs, classes, functions,
etc.). You should also write unit tests to verify each component works
on its own.

You are welcome to use any part of the C++ standard library in your top-level classifier
application. See our [C++ Standard Library Containers](containers.html) reference for a description of several containers and examples of how to use them. In particular, `std::map` and `std::set` will be
useful for this project.

### Example

We've provided full example output for a small input (`train_small.csv` and `test_small.csv`).  The output is in `test_small.out.correct`.  The output in train-only mode is in `train_small_train_only.out.correct`, here we've indicated train-only output with "(TRAIN-ONLY)".  Some lines are indented by two spaces.

To run this example at the command line in train-only mode:
```console
$ make classifier.exe
$ ./classifier.exe train_small.csv
```

To run with test data and generate predictions:
```console
$ ./classifier.exe train_small.csv test_small.csv
```

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Debug output differences with `diff -y -B`, which shows differences side-by-side and ignores whitespace.  We'll use the `less` pager so we can scroll through the long terminal output.  Press `q` to quit.
```console
$ make classifier.exe
$ ./classifier.exe train_small.csv > train_small_train_only.out
$ diff -y -B train_small_train_only.out train_small_train_only.out.correct | less  # q to quit
```
</div>

Add this line at the beginning of your `main` function to set floating
point precision:

```c++
cout.precision(3);
```
{: data-variant="no-line-numbers" }

First, print information about the training data:

- (TRAIN-ONLY) Line-by-line, the label and content for each training document.
  ```
  training data:
    label = euchre, content = can the upcard ever be the left bower
    label = euchre, content = when would the dealer ever prefer a card to the upcard
    label = euchre, content = bob played the same card twice is he cheating
    ...
    label = calculator, content = does stack need its own big three
    label = calculator, content = valgrind memory error not sure what it means
  ```
  {: data-variant="no-line-numbers" }
- The number of training posts.
  ```
  trained on 8 examples
  ```
  {: data-variant="no-line-numbers" }
- (TRAIN-ONLY) The vocabulary size (the number of unique words in all training content).
  ```
  vocabulary size = 49
  ```
  {: data-variant="no-line-numbers" }
- An extra blank line

In train-only mode, also print information about the classifier
trained on the training posts. Whenever classes or words are listed,
they are in alphabetic order.

- (TRAIN-ONLY) The classes in the training data, and the number of examples for each.
  ```
  classes:
    calculator, 3 examples, log-prior = -0.981
    euchre, 5 examples, log-prior = -0.47
  ```
  {: data-variant="no-line-numbers" }
- (TRAIN-ONLY) For each label, and for each word that occurs for that label: The
  number of posts with that label that contained the word, and the
  log-likelihood of the word given the label.
  ```
  classifier parameters:
    calculator:assert, count = 1, log-likelihood = -1.1
    calculator:big, count = 1, log-likelihood = -1.1
    ...
    euchre:twice, count = 1, log-likelihood = -1.61
    euchre:upcard, count = 2, log-likelihood = -0.916
    ...
   ```
   {: data-variant="no-line-numbers" }
- (TRAIN-ONLY) An extra blank line

Finally, if a test file is provided, use the classifier to predict
classes for each example in the testing data. Print information about
the test data as well as these predictions.

- Line-by-line, the "correct" label, the predicted label and its log-probability
  score, and the content for each test. Insert a blank line after each for
  readability.
  ```
  test data:
    correct = euchre, predicted = euchre, log-probability score = -13.7
    content = my code segfaults when bob is the dealer

    correct = euchre, predicted = calculator, log-probability score = -12.5
    content = no rational explanation for this bug

    correct = calculator, predicted = calculator, log-probability score = -13.6
    content = countif function in stack class not working
  ```
  {: data-variant="no-line-numbers" }
- The number of correct predictions and total number of test posts.
  ```
  performance: 2 / 3 posts predicted correctly
  ```
  {: data-variant="no-line-numbers" }

The last thing printed should be a newline character.

### Accuracy

In case you're curious, here's the accuracy for the large datasets. Not too bad!

|                               Command                                |  Accuracy   |
| -------------------------------------------------------------------- | ----------- |
| `./classifier.exe w16_projects_exam.csv sp16_projects_exam.csv`              | 245 / 332   |
| `./classifier.exe w14-f15_instructor_student.csv w16_instructor_student.csv` | 2602 / 2988 |

### Efficiency

While efficiency is not a primary goal for this project, you should
aim for your code to run the largest test case above in no more than a
minute. Some common causes of slowdown you should avoid:

- Processing a post more than once (including reading it more than
  once or saving all the data in a vector).
- Iterating over a map to find something rather than using `[]` or `find()`.
- Passing strings, pairs, or containers by value.
- Iterating by value in a range-based for loop.

Refer to the [Project 2 perf
tutorial](https://eecs280staff.github.io/p2-cv/perf.html)
for instructions on how to use the `perf` tool to identify slow
functions.

## Submission and Grading

Submit these files to the [autograder](https://autograder.io).
  - `classifier.cpp`

This project will be autograded for correctness and programming style. See the [style checking
tutorial](https://eecs280staff.github.io/p1-stats/setup_style.html)
for the criteria and how to check your style automatically on CAEN.

### Testing

Run all the tests.  For this project, the tests only consist of system tests.

```console
$ make test
```

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Run commands in parallel with `make -j`.
```console
$ make -j4 test
```
{: data-variant="no-line-numbers" }
</div>

### Requirements and Restrictions

| DO | DO NOT |
| -- | ------ |
| Put all top-level application code in `classifier.cpp`. | Create additional files other than `classifier.cpp`. |
| Create any ADTs or functions you wish for your top-level classifier application. | Write everything in the `main()` function. |
| Use any part of the C++ standard library for your top level classifier application, including `map` and `set`. | Write your own implementation of maps and sets -- they will likely be too slow. |
| Follow course style guidelines. | Use non-const static or global variables. |
| [Check for undefined behavior](https://eecs280staff.github.io/p1-stats/setup_asan.html#p1-stats) using address sanitizer and other tools | "It runs fine on my machine!" |


## Acknowledgments

Andrew DeOrio and James Juett wrote the original project and specification. Amir Kamil contributed to code structure, style, and implementation details. This project was developed for EECS 280, Fall 2016 at the University of Michigan. The classifer was forked into a separate project in Fall 2024.
