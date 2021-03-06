/**************************************************************************************************/
/*!
@file   ProgressBar.hpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention � Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/
#pragma once

#define NOMINMAX
#include "Window.hpp"
#include <string>

#include "Threading.hpp"

class ProgressBar : public RoutineObject
{
  public:
    ProgressBar( const std::string &title );
    virtual ~ProgressBar( void ) throw();

    void SetRange( unsigned short min, unsigned short max );
    void SetStep( unsigned short step );

    void Step( void );

  private:
    void Destroy( void );

    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void ) throw();

    virtual void FlushThread( void );


  private:
      // Main window info of the progress bar
    WNDCLASSEX window_;
    HWND win_;

      /// Handle to the progress bar (which is a child of the window.)
    HWND pbHandle_;

    std::string title_;

    Event quit_;
};    // ProgressBar

