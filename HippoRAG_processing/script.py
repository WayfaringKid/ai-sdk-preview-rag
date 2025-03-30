import os
import glob
import re
import json
import multiprocessing

def main():
    from hipporag import HippoRAG

    markdown_dir = os.path.join(os.path.dirname(__file__), '..', 'projectspecs', 'p3_euchre.md')
    query_dir = os.path.join(os.path.dirname(__file__), '..', 'discussionthreads')

    docs = []

    with open(markdown_dir, 'r', encoding='utf-8') as f:
        content = f.read()
        docs.append(content)
    print("=== Docs loaded from 'projectspecs' ===")
    for i, doc in enumerate(docs):
        snippet = doc[:100].replace("\n", "\\n")
        print(f"Doc[{i}] {snippet}...")

    save_dir = 'outputs'
    llm_model_name = 'gpt-4o-mini'
    embedding_model_name = 'facebook/contriever'

    hipporag = HippoRAG(
        save_dir=save_dir, 
        llm_model_name=llm_model_name,
        embedding_model_name=embedding_model_name
    )

    hipporag.index(docs=docs)

    queries = []
    json_query_file = os.path.join(query_dir, "sp24_project3_plaintext_parsed.json")
    if os.path.isfile(json_query_file):
        with open(json_query_file, "r", encoding="utf-8") as f:
            data = json.load(f) 
            for item in data:
                q = item.get("question", "").strip()
                if q:
                    queries.append(q)
    else:
        print(f"[WARNING] JSON file not found: {json_query_file}")

    print("=== Queries loaded from 'sp24_parsed.json' ===")
    for i, q in enumerate(queries):
        snippet = q.replace("\n", "\\n")
        print(f"Q[{i}]: {snippet}")

    rag_results = hipporag.rag_qa(queries=queries)
    answers = rag_results[0]  # List of answer strings
    metadata = rag_results[1]  # List of dicts with token stats


    print("=== RAG QA (One step) ===\n")
    clean_list = []
    for i, ans in enumerate(rag_results[0]):
        print(f"--- Result {i + 1} ---")
        print(f"Question: {ans.question}")
        print(f"Answer:   {ans.answer}")
        print("----")

        entry = {
            "question": ans.question,
            "answer": ans.answer
        }
        clean_list.append(entry)

    with open("qa_results.json", "w", encoding="utf-8") as f:
        json.dump(clean_list, f, ensure_ascii=False, indent=2)

    print("[*] Done, see qa_results.json")

if __name__ == "__main__":
    multiprocessing.set_start_method("fork", force=True)

    main()
