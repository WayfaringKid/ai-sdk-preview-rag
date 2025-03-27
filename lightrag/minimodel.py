import os
import csv
import json
import asyncio
from lightrag import LightRAG, QueryParam
from lightrag.llm.openai import gpt_4o_mini_complete, gpt_4o_complete, openai_embed
from lightrag.kg.shared_storage import initialize_pipeline_status

WORKING_DIR = "/Users/a1111/Desktop/University of Michigan/2025 winter/CSE592/Project/LightRAG"

if not os.path.exists(WORKING_DIR):
    os.mkdir(WORKING_DIR)

async def initialize_rag():
    # Create the LightRAG instance with the desired LLM
    rag = LightRAG(
        working_dir=WORKING_DIR,
        embedding_func=openai_embed,
        llm_model_func=gpt_4o_mini_complete,  # or gpt_4o_complete if you prefer GPT-4o
    )

    # Initialize storage and pipeline
    await rag.initialize_storages()
    await initialize_pipeline_status()

    return rag

def main():
    # 1. Initialize RAG (async)
    rag = asyncio.run(initialize_rag())

    # 2. Insert p3_euchre.md as augmentation data
    with open("./p3_euchre.md", "r", encoding="utf-8") as f:
        rag.insert(f.read())

    # 3. Parse sp24_project3_plaintext_parsed.json to get questions + answers
    with open("sp24_project3_plaintext_parsed.json", "r", encoding="utf-8") as f:
        data = json.load(f)  # each element has {"question": "...", "answers": ["...", "..."]}

    # 4. Prepare a CSV to store results
    output_csv = "rag_output.csv"
    with open(output_csv, "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["id", "question", "response", "answer"])

        # 5. For each question in the JSON, query LightRAG and write a row
        for idx, item in enumerate(data, start=1):
            question_text = item.get("question", "").strip()
            # We can combine all answers into one string (e.g., separated by "; ")
            # or just the first answer. Let's combine them for completeness:
            answers_list = item.get("answers", [])
            original_answer = "; ".join(answers_list)

            # Query the model in "hybrid" mode
            response = rag.query(
                question_text,
                param=QueryParam(mode="hybrid")
            )

            # Write row: id, question, response, original correct answer
            writer.writerow([idx, question_text, response, original_answer])

    print(f"Done! Results written to {output_csv}")

if __name__ == "__main__":
    main()