import os
import glob
import re
import json
import multiprocessing

def main():
    # 在 main 内部再导入 HippoRAG，避免模块顶层就触发多进程创建
    from hipporag import HippoRAG

    # 假设当前脚本与 "projectspecs"、"discussionthreads" 文件夹同级
    markdown_dir = os.path.join(os.path.dirname(__file__), '..', 'projectspecs')
    query_dir = os.path.join(os.path.dirname(__file__), '..', 'discussionthreads')

    # 1) 读取 docs
    docs = []
    markdown_files = glob.glob(os.path.join(markdown_dir, "*.md"))
    for md_file in markdown_files:
        with open(md_file, 'r', encoding='utf-8') as f:
            content = f.read()
            docs.append(content)

    print("=== Docs loaded from 'projectspecs' ===")
    for i, doc in enumerate(docs):
        snippet = doc[:100].replace("\n", "\\n")
        print(f"Doc[{i}] {snippet}...")

    # 2) 初始化 HippoRAG
    save_dir = 'outputs'
    llm_model_name = 'gpt-4o-mini'        # 如要真实连接OpenAI，请改成 'gpt-3.5-turbo' 或 'gpt-4'
    embedding_model_name = 'nvidia/NV-Embed-v2'

    hipporag = HippoRAG(
        save_dir=save_dir, 
        llm_model_name=llm_model_name,
        embedding_model_name=embedding_model_name
    )

    # 3) 对 docs 进行索引
    hipporag.index(docs=docs)

    # 4) 读取 queries（以 sp24_parsed.json 为例）
    queries = []
    json_query_file = os.path.join(query_dir, "sp24_parsed.json")
    if os.path.isfile(json_query_file):
        with open(json_query_file, "r", encoding="utf-8") as f:
            data = json.load(f)  # 假设 data 是一个列表
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

    # 5) 真跑检索&问答
    retrieval_results = hipporag.retrieve(queries=queries, num_to_retrieve=2)
    qa_results = hipporag.rag_qa(retrieval_results)
    print("=== QA Results (retrieval + QA) ===")
    for i, ans in enumerate(qa_results):
        print(f"Question: {queries[i]}")
        print(f"Answer:   {ans}")
        print("----")

    # 一步到位 RAG QA
    rag_results = hipporag.rag_qa(queries=queries)
    print("=== RAG QA (One step) ===")
    for i, ans in enumerate(rag_results):
        print(f"Question: {queries[i]}")
        print(f"Answer:   {ans}")
        print("----")

    # 6) 把 Q&A 结果写入到 JSON 文件
    q_and_a_list = []
    for i, ans in enumerate(rag_results):
        entry = {
            "question": queries[i],
            "answer": ans
        }
        q_and_a_list.append(entry)

    output_path = "qa_results.json"
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(q_and_a_list, f, ensure_ascii=False, indent=2)

    print(f"[*] Q&A results saved to {output_path}")


if __name__ == "__main__":
    # 对于macOS下多进程，可以用 'fork' 替代 'spawn'，避免冲突
    multiprocessing.set_start_method("fork", force=True)

    main()
