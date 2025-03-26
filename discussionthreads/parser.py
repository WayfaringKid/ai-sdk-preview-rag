import os
import re
import sys
import json

def is_plain_text(entry):
    '''Returns True if document has no images or attachments'''
    doc = entry.get('document', '').lower()
    return not any(tag in doc for tag in ['<img', '<image', 'href=', 'attachment'])

def get_question_and_answers(json_file):
    '''returns a list of plain-text Project 3 questions and their answers'''
    result = []
    with open(json_file) as f:
        data = json.load(f)
        for entry in data:
            if (
                entry.get('type') == 'question' and
                entry.get('category', '').startswith("Project 3") and
                is_plain_text(entry)
            ):
                question = re.sub(r'\n', ' ', entry.get('text', '')).strip()
                answers = [ans.get('text', '').strip() for ans in entry.get('answers', []) if ans.get('text')]
                if question and answers:
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
        if result_list:
            out_file = re.sub(r'\.json$', '_project3_plaintext_parsed.json', json_file)
            write_json_file(result_list, out_file)
            print(f'Wrote {out_file}')

if __name__ == '__main__':
    main()
