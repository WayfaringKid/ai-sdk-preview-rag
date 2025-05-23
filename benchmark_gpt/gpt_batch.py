import os
import json
import openai
from dotenv import load_dotenv

# Load OpenAI API key
load_dotenv()
openai.api_key = ""

# Paths
markdown_dir = os.path.join(os.path.dirname(__file__), '..', 'projectspecs', 'p3_euchre.md')
query_file_path = os.path.join(os.path.dirname(__file__), '..', 'discussionthreads', 'w25_project3_plaintext_parsed.json')

# Load the project spec doc
with open(markdown_dir, 'r', encoding='utf-8') as f:
    docs_context = f.read()

# Load starter files
starter_dir = os.path.join(os.path.dirname(__file__), '..', 'starterfiles', 'p3-starter-files-USE-THESE')
starter_context = ""

for root, _, files in os.walk(starter_dir):
    for filename in files:
        file_path = os.path.join(root, filename)
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
                relative_path = os.path.relpath(file_path, starter_dir)
                starter_context += f"\n\n===== FILE: {relative_path} =====\n{content}"
        except Exception as e:
            print(f"⚠️ Skipped {file_path}: {e}")

# Load queries from JSON
try:
    with open(query_file_path, 'r', encoding='utf-8') as f:
        query_data = json.load(f)
except Exception as e:
    print(f"❌ Failed to read query file: {e}")
    exit(1)

queries = [item["question"] for item in query_data]
print(f"✅ Loaded {len(queries)} queries from JSON.")

if not queries:
    print("❌ No queries found. Exiting.")
    exit(1)

# Start with the initial conversation history
conversation = [
    {
        "role": "system",
        "content": "You are an Instructional Aide (IA) for EECS 280 at the University of Michigan. "
                   "You will help students by answering questions based on the project specs and starter files."
    },
    {
        "role": "user",
        "content": f"Here are the project specs from EECS 280:\n\n{docs_context}\n\n"
                f"And here are the starter files:\n{starter_context}"
    }
]

# Q&A loop
qa_pairs = []

for i, query in enumerate(queries):
    print(f"\n=== Processing Q[{i}] ===")
    try:
        # Add query to conversation
        conversation.append({"role": "user", "content": query})

        # Get response
        response = openai.chat.completions.create(
            model="gpt-4o-mini",
            messages=conversation,
            temperature=0.3
        )
        answer = response.choices[0].message.content

        # Append assistant response to the conversation
        conversation.append({"role": "assistant", "content": answer})

        # Save the Q&A pair
        qa_pairs.append({"question": query, "answer": answer})
        print(f"✅ Answered Q[{i}]")
    except Exception as e:
        print(f"❌ Error on Q[{i}]: {e}")
        qa_pairs.append({"question": query, "answer": f"[ERROR] {str(e)}"})


# Save answers to JSON
output_path = os.path.join(os.path.dirname(__file__), "answers.json")
try:
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(qa_pairs, f, indent=2, ensure_ascii=False)
    print(f"\n✅ All answers saved to: {output_path}")
except Exception as e:
    print(f"❌ Failed to save answers: {e}")