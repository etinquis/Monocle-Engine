#include "Rect.h"
 
namespace Monocle
{
	Rect::Rect()
	{
	}
 
	Rect::Rect( const Vector2 &position, const Vector2 &size ):
		position( position ), size( size )
	{
 
	}
 
	Rect::Rect( Vector2::CoordType x, Vector2::CoordType y, Vector2::CoordType w, Vector2::CoordType h ):
		position( Vector2( x, y ) ), size( Vector2( w, h ) )
	{
 
	}
 
	Rect::Rect( const Rect &other ):
		position( other.position ), size( other.size )
	{
 
	}
 
	Vector2 Rect::GetTopLeft() const
	{
		return position;
	}
 
	Vector2 Rect::GetTopRight() const
	{
		return Vector2( GetRight(), GetTop() );
	}
 
	Vector2 Rect::GetBottomRight() const
	{
		return Vector2( GetRight(), GetBottom() );
	}
 
	Vector2 Rect::GetBottomLeft() const
	{
		return Vector2( GetLeft(), GetBottom() );
	}
 
	Vector2 Rect::GetPosition() const
	{
		return position;
	}
 
	Vector2 Rect::GetSize() const
	{
		return size;
	}
 
	Vector2::CoordType Rect::GetWidth() const
	{
		return GetRight() - GetLeft();
	}
 
	Vector2::CoordType Rect::GetHeight() const
	{
		return GetBottom() - GetTop();
	}
 
	Vector2::CoordType Rect::GetRight() const
	{
		return position.x + size.x;
	}
 
	Vector2::CoordType Rect::GetLeft() const
	{
		return position.x;
	}
 
	Vector2::CoordType Rect::GetTop() const
	{
		return position.y;
	}
 
	Vector2::CoordType Rect::GetBottom() const
	{
		return position.y + size.y;
	}
 
	void Rect::SetPosition( const Vector2 &newPos )
	{
		position = Vector2( newPos );
	}
 
	void Rect::SetPosition( Vector2::CoordType x, Vector2::CoordType y )
	{
		position = Vector2( x, y );
	}
 
	void Rect::SetPositionOffset( const Vector2 &posOffset )
	{
		position += posOffset;
	}
 
	void Rect::SetPositionOffset( Vector2::CoordType x, Vector2::CoordType y )
	{
		position += Vector2( x, y );
	}
 
	void Rect::SetSize( const Vector2 &newSize )
	{
		size = Vector2( newSize );
	}
 
	void Rect::SetSize( Vector2::CoordType w, Vector2::CoordType h )
	{
		size = Vector2( w, h );
	}
 
	void Rect::Scale( Vector2 scale )
	{
		size = Vector2( size.x * scale.x, size.y * scale.y );
	}
 
	void Rect::Scale(Vector2::CoordType scale)
	{
		size *= scale;
	}
 
	void Rect::Scale( Vector2::CoordType x, Vector2::CoordType y )
	{
		size = Vector2( size.x * x, size.y * y );
	}
}
 
 
