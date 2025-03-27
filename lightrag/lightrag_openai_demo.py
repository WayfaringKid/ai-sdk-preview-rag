import os
import asyncio
from lightrag import LightRAG, QueryParam
from lightrag.llm.openai import gpt_4o_mini_complete, gpt_4o_complete, openai_embed
from lightrag.kg.shared_storage import initialize_pipeline_status
from parse import parse_qa_json

WORKING_DIR = "/Users/a1111/Desktop/University of Michigan/2025 winter/CSE592/Project/LightRAG"

if not os.path.exists(WORKING_DIR):
    os.mkdir(WORKING_DIR)


async def initialize_rag():
    rag = LightRAG(
        working_dir=WORKING_DIR,
        embedding_func=openai_embed,
        llm_model_func=gpt_4o_mini_complete,
        # llm_model_func=gpt_4o_complete
    )

    await rag.initialize_storages()
    await initialize_pipeline_status()

    return rag


def main():
    # Initialize RAG instance
    rag = asyncio.run(initialize_rag())

    with open("./p4-classifier.md", "r", encoding="utf-8") as f:
        rag.insert(f.read())

    # Perform naive search
    print(
        rag.query(
            "What is the due date of this project?", param=QueryParam(mode="naive")
        )
    )

    # Perform local search
    print(
        rag.query(
            "What is the due date of this project?", param=QueryParam(mode="local")
        )
    )

    # Perform global search
    print(
        rag.query(
            "What is the due date of this project?", param=QueryParam(mode="global")
        )
    )

    # Perform hybrid search
    print(
        rag.query(
            "What is the due date of this project?", param=QueryParam(mode="hybrid")
        )
    )


if __name__ == "__main__":
    main()
