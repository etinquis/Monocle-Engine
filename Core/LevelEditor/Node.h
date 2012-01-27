#pragma once

#include "../Entity.h"

namespace Monocle
{
	//! /brief Linked list marker. Used to describe paths.
	class Node : public Entity
	{
	public:
		Node(const Node &node);
		Node();
		Node(const Vector2 &position);
		Entity* Clone();
		void Update();
		void Render();
		void Disconnect();
		void TakeOut();

		void SetNext(Node *node);
		void InsertNext(Node *node);

		Node *GetNext();
		Node *GetPrev();

		virtual void SaveTo(FileNode *fileNode);
		virtual void LoadFrom(FileNode *fileNode);

		int variant;
		
	private:
		Node *next;
		Node *prev;
	};
}

