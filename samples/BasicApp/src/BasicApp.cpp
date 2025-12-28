#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

// typedef std::pair<vec2, vec2> line;


// We'll create a new Cinder Application by deriving from the App class.
class BasicApp : public App {
  public:
	// Cinder will call 'mouseDrag' when the user moves the mouse while holding one of its buttons.
	// See also: mouseMove, mouseDown, mouseUp and mouseWheel.
	void mouseUp( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	// Cinder will call 'keyDown' when the user presses a key on the keyboard.
	// See also: keyUp.
	void keyDown( KeyEvent event ) override;

	// Cinder will call 'draw' each time the contents of the window need to be redrawn.
	void draw() override;

  private:
	// This will maintain a list of points which we will draw line segments between
	//std::vector<vec2> mPoints;
	//std::vector<line> mLines;
	//line currentLine;
	std::vector<std::vector<vec2>> mCurves;
	std::vector<vec2> mCurrentCurve;
};

void prepareSettings( BasicApp::Settings* settings )
{
	settings->setMultiTouchEnabled( false );
}

void BasicApp::mouseUp( MouseEvent event )
{
	mCurves.push_back( mCurrentCurve );
	mCurrentCurve.clear();
}
void BasicApp::mouseDrag( MouseEvent event )
{
	mCurrentCurve.push_back(event.getPos());
}

void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' ) {
		// Toggle full screen when the user presses the 'f' key.
		setFullScreen( ! isFullScreen() );
	}
	else if( event.getCode() == KeyEvent::KEY_SPACE ) {
		// Clear the list of points when the user presses the space bar.
		//mPoints.clear();
		//mLines.clear();
		mCurves.clear();
	}
	else if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
		// Exit full screen, or quit the application, when the user presses the ESC key.
		if( isFullScreen() )
			setFullScreen( false );
		else
			quit();
	}
}

void drawCurve( const std::vector<vec2>& curve )
{
	for( const vec2& point : curve ) {
		gl::vertex( point );
	}
}

void BasicApp::draw()
{
	// Clear the contents of the window. This call will clear
	// both the color and depth buffers. 
	gl::clear( Color::gray( 1.0f ) );

	// Set the current draw color to orange by setting values for
	// red, green and blue directly. Values range from 0 to 1.
	// See also: gl::ScopedColor
	gl::color( 1.0f, 0.5f, 0.25f );

	// We're going to draw a line through all the points in the list
	// using a few convenience functions: 'begin' will tell OpenGL to
	// start constructing a line strip, 'vertex' will add a point to the
	// line strip and 'end' will execute the draw calls on the GPU.
	//gl::begin( GL_LINE_STRIP );
	std::for_each( mCurves.begin(), mCurves.end(), []( const std::vector<vec2>& c ) { drawCurve(c); } );
	gl::end();
}


	// This line tells Cinder to actually create and run the application.
CINDER_APP( BasicApp, RendererGl, prepareSettings )
