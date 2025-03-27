import json

def parse_qa_json(file_path: str):
    """
    Reads the JSON file at file_path, where each object has:
        {
          "question": "string",
          "answers": ["answer1", "answer2", ...]
        }
    Returns a list of strings, each containing:
        "Question: <question>\nAnswer 1: <answer1>\nAnswer 2: <answer2>\n..."
    """
    with open(file_path, "r", encoding="utf-8") as f:
        data = json.load(f)

    parsed_chunks = []
    for item in data:
        question_text = item.get("question", "").strip()
        answers_list = item.get("answers", [])

        # Build a combined string for each question–answers pair
        # e.g. "Question: ...\nAnswer 1: ...\nAnswer 2: ...\n..."
        combined_str = f"Question: {question_text}"
        for i, ans in enumerate(answers_list, start=1):
            ans = ans.strip()
            combined_str += f"\nAnswer {i}: {ans}"

        parsed_chunks.append(combined_str)

    return parsed_chunks


def main():

    # Read the markdown file containing Q&A
    with open("./w25.md", "r", encoding="utf-8") as f:
        md_text = f.read()

    # Parse Q&A into chunks
    qapairs = parse_qa_json(md_text)

    # Insert each chunk
    for chunk in qapairs[0]:
        print(chunk)

if __name__ == "__main__":
    main()