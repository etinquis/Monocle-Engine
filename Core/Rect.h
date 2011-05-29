#pragma once
 
#include "Vector2.h"
 
 
namespace Monocle
{
	//!
	//! \brief Describes a rectangle.
	//!
	class Rect
	{
	public:
		Rect();
		Rect( const Vector2 &position, const Vector2 &size );
		Rect( Vector2::CoordType x, Vector2::CoordType y, Vector2::CoordType w, Vector2::CoordType h );
		Rect( const Rect &other );
 
		//! Returns the position of the top left point of the Rectangle.
		//! Equivalent to GetPosition().
		Vector2 GetTopLeft() const;
		Vector2 GetTopRight() const;
		Vector2 GetBottomRight() const;
		Vector2 GetBottomLeft() const;
 
		//! Returns the current position of the rectangle
		Vector2 GetPosition() const;
		//! Returns the current size of the rectangle
		Vector2 GetSize() const;
 
		Vector2::CoordType GetWidth() const;
		Vector2::CoordType GetHeight() const;
 
		Vector2::CoordType GetRight() const;
		Vector2::CoordType GetLeft() const;
		Vector2::CoordType GetTop() const;
		Vector2::CoordType GetBottom() const;
 
		//! Changes the position of the top left corner of the rectangle.
		void SetPosition( const Vector2 &newPos );
		void SetPosition( Vector2::CoordType x, Vector2::CoordType y );
		//! Changes the position of the rectangle relative to its current position.
		void SetPositionOffset( const Vector2 &posOffset );
		void SetPositionOffset( Vector2::CoordType x, Vector2::CoordType y );
		//! Resizes the rectangle
		//! \param newsize The size of the resulting rectangle.  X and Y must be positive.
		void SetSize( const Vector2 &newsize );
		void SetSize( Vector2::CoordType w, Vector2::CoordType h );
 
		void Scale( Vector2 size );
		void Scale( Vector2::CoordType size );
		void Scale( Vector2::CoordType x, Vector2::CoordType y );
 
	private:
		Vector2 position;
		Vector2 size;
	};
}
 
 
