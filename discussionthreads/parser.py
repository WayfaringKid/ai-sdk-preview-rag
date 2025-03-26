import os
import re
import sys
import json

def get_question_and_answers(json_file):
    '''returns a list of dictionaries with project-prefixed question and list of answers'''
    result = []
    with open(json_file) as f:
        data = json.load(f)
        for key in data:
            if key['type'] == 'question' and key['category'].startswith("Project"):
                question = re.sub(r'\n', ' ', key['text']).strip()
                answers = [ans['text'] for ans in key.get('answers', [])]
                if answers:
                    result.append({
                        "question": f"project_{question}",
                        "answers": answers
                    })
    return result

def write_json_file(result_list, out_file):
    '''writes the structured data to a JSON file'''
    with open(out_file, 'w') as f:
        json.dump(result_list, f, indent=2)

def main():
    if len(sys.argv) != 1:
        print('Usage: python3 parser.py')
        sys.exit(1)
    for json_file in os.listdir('.'):
        if not json_file.endswith('.json'):
            continue
        result_list = get_question_and_answers(json_file)
        out_file = re.sub(r'\.json$', '_parsed.json', json_file)
        write_json_file(result_list, out_file)
        print(f'Wrote {out_file}')

if __name__ == '__main__':
    main()
