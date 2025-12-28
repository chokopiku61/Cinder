#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

// typedef std::pair<vec2, vec2> line;


// We'll create a new Cinder Application by deriving from the App class.
class BasicApp : public App {
  public:
	void mouseUp( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	void mouseDown( MouseEvent event ) override;
	// Cinder will call 'keyDown' when the user presses a key on the keyboard.
	// See also: keyUp.
	void keyDown( KeyEvent event ) override;

	// Cinder will call 'draw' each time the contents of the window need to be redrawn.
	void draw() override;
	
  private:
	bool paletteClick( const vec2& pos );
	Color mCurrentColor = Color::gray(0);
	std::vector<std::pair<std::vector<vec2>, Color>> mCurves; //Пришлось добавить цвет, иначе цвет изменяется еще и устарых кривых
	std::vector<vec2> mCurrentCurve;
	const std::vector<Color> colors = { Color( 1.0f, 0.0f, 0.0f ), 
		Color( 1.0f, 1.0f, 0.0f ), 
		Color( 0.0f, 1.0f, 0.0f ), 
		Color( 0.0f, 0.0f, 1.0f ), 
	    Color( 1.0f, 0.0f, 1.0f ), 
		Color( 0.0f, 1.0f, 1.0f ) };
	std::vector<std::pair<vec2, vec2>> paletteRects; // Вектор точек верхней левой и правой нижней границы прямоугольников палитры
	const float paletteX = 15.0f;
	const float paletteY = 15.0f;
	const float size = 42.0f;
};


void prepareSettings( BasicApp::Settings* settings )
{
	settings->setMultiTouchEnabled( false );
}

void BasicApp::mouseUp( MouseEvent event )
{
	if( ! mCurrentCurve.empty() ) {
		mCurves.emplace_back( mCurrentCurve, mCurrentColor ); //Сохраняем   линию и цвет
		mCurrentCurve.clear();
	}
}
void BasicApp::mouseDrag( MouseEvent event )
{
	mCurrentCurve.push_back(event.getPos());
}

void BasicApp::mouseDown( MouseEvent event )
{
	paletteClick( event.getPos() );
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

void drawCurve( const std::pair<std::vector<vec2>, Color> curve )
{
	gl::color( curve.second );
	/*for( const vec2& point : curve.first ) {
		gl::vertex( point );
	}*/

	for( int i = 0; i < curve.first.size() - 1; ++i ) {
		gl::drawLine( curve.first[i], curve.first[i + 1] );
	}
}

void BasicApp::draw()
{
	gl::clear( Color::gray( 1.0f ) );
	
	float pX = paletteX;
	float pY = paletteY;

	for( const auto& color : colors ) {
		gl::color( color );
		gl::drawSolidRect( Rectf( pX, pY, pX + size, pY + size ) );
		paletteRects.push_back( { { pX, pY }, { pX + size, pY + size } } );
		pY += size + 10; // Отступ 
	}
	
	gl::color( mCurrentColor );

	std::for_each( mCurves.begin(), mCurves.end(), []( const auto& c ) { drawCurve(c); } );
	gl::end();
}

bool BasicApp::paletteClick( const vec2& pos )
{
	for( int i = 0; i < colors.size(); ++i ) {
		std::pair<vec2, vec2> cur = paletteRects[i];
		if( ( pos.y >= cur.first.y ) && ( pos.y <= cur.second.y ) && ( pos.x >= cur.first.x ) && ( pos.x <= cur.second.x ) ) {
			mCurrentColor = colors[i];
			return true;
		}
	}
	return false;
}

	// This line tells Cinder to actually create and run the application.
CINDER_APP( BasicApp, RendererGl, prepareSettings )
