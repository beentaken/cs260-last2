#include "Precompiled.h"
#include "Text.h"
#include "Transform.h"
#include "Graphics.h"
#include "Camera.h"
#include "DebugDraw.h"
#include <algorithm>

namespace Framework
{
/**************************************************************************************************/
/**************************************************************************************************/
  Text::Text( void ) : Next(NULL), Prev(NULL), pFont_(NULL), height_(0), width_(0), screen_(false)
  {
  }

/**************************************************************************************************/
/**************************************************************************************************/
  Text::~Text( void )
  {
    GRAPHICS->TextList.erase( this );
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void Text::Initialize( void )
  {
    transform = GetOwner()->has( Transform );

    GRAPHICS->TextList.push_back( this );
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void Text::Serialize( ISerializer &stream )
  {
    int red, green, blue, alpha;
    std::string textlocation;

    StreamRead( stream, textlocation );

      // Convert the string read in to uppercase letters for comparison reasons.
    std::transform( textlocation.begin(), textlocation.end(), textlocation.begin(), toupper );

      // Determine what space this text should be displayed in!
    if ( textlocation == "SCREEN" )
    {
      screen_ = true;
    }
    else if ( textlocation == "WORLD" )
    {
      screen_ = false;
    }
    else
    {
      ErrorIf( true, "Error: Specify \"Screen\" or \"World\" space for the Text component." );
    }

    StreamRead( stream, height_ );
    StreamRead( stream, width_ );
    StreamLine( stream, fontname_ );    // (i.e. Comic Sans MS, Arial, Courier New)

      // Read in the color of the texture! (Note, values between 0 and 255)
    StreamRead( stream, red );
    StreamRead( stream, green );
    StreamRead( stream, blue );
    StreamRead( stream, alpha );

    StreamRead( stream, text_ );

    SetColor( red, green, blue, alpha );
    ChangeFont( width_, height_, fontname_ );
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void Text::ChangeFont( unsigned width, unsigned height, const std::string &fontname )
  {
    width_ = width;
    height_ = height_;
    fontname_ = fontname;

      // Get the font this text is going to use from graphics.
    pFont_ = GRAPHICS->GetFont( fontname_, width_, height_ );
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void Text::SetColor( unsigned char red, unsigned char green, unsigned char blue,
    unsigned char alpha )
  {
    color_ = D3DCOLOR_ARGB( alpha, red, green, blue );
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void Text::SetText( const char *msg, ... )
  {
	  const int BufferSize = 1024;
	  char FinalMessage[BufferSize];

      // Gets us sprintf functionallity
	  va_list args;
	  va_start(args, msg);
	  vsnprintf_s( FinalMessage, BufferSize, _TRUNCATE, msg, args );
	  va_end(args);

      // Set that to be the text that we are going to draw on the screen
    text_ = FinalMessage;
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void Text::Draw( void )
  {
    Vec2 pos;

    if ( screen_ )
    {
      pos = transform->Position;
    }
    else
    {
      pos = GRAPHICS->WorldToScreenSpace( transform->Position );
    }

    RECT rect;
    rect.left = static_cast<long>( pos.x );
    rect.top  = static_cast<long>( pos.y );

      // Calculate the RECT coords
    pFont_->DrawText( NULL, text_.c_str(), text_.size(), &rect, DT_CALCRECT, color_ );

      // Actually draws the text!
    pFont_->DrawText( NULL, text_.c_str(), -1, &rect, 0, color_ );
  }

}   // namespace Framework


