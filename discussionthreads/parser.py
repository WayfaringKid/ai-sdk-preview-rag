''' Gets the question and answer from the discussion thread .json file and prints them nicely in a new md file. '''
import os
import re
import sys

import json

def get_question_and_answers(json_file):
    '''returns a list of tuples with the question and answers'''
    result = []
    with open(json_file) as f:
        data = json.load(f)
        for key in data:
            if key['type'] == 'question':
                question = key['text']
                answers = key['answers']

        # answers is a list of other json objects
                answer_list = []
                for key1 in answers:
                    answer = key1['text']
                    answer_list.append(answer)
                result.append((question, answer_list))
    return result

def write_md_file(result_list, md_file):
    '''writes the question and answer to a markdown file'''
    with open(md_file, 'w') as f:
        i = 0
        for elt in result_list:
            question = elt[0]
            # strip question of newlines
            question = re.sub(r'\n', ' ', question)
            answers = elt[1]
            f.write(f'# **Question {i}**: {question}\n\n')
            j = 0
            if len(answers) == 0:
                f.write('**No answers provided**\n')
                f.write("\n\n")
                continue
            if len(answers) == 1:
                f.write(f'**Answer**: {answers[0]}\n')
                f.write("\n\n")
                continue
            for answer in answers:
                f.write(f'**Answer {j}**: {answer}\n')
                j += 1
            f.write("\n\n")
            i += 1


def main():
    if len(sys.argv) != 1:
        print('Usage: python3 parser.py')
        sys.exit(1)
    # convert any json files from inside the current directory to a markdown file
    for json_file in os.listdir('.'):
        if not json_file.endswith('.json'):
            continue
        result_list = get_question_and_answers(json_file)
        md_file = re.sub(r'\.json$', '.md', json_file)
        write_md_file(result_list, md_file)
        print(f'Wrote {md_file}')

if __name__ == '__main__':
    main()