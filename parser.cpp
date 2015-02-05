	    int n_io = _open( "DATA.TXT", _O_TEXT );
		while ( ! eof(  n_io ) )
		{
			char s[ 2000 ];
			char x = char ( 0 );
			int n = 0;
			while ( x != '\n'  )
			{
			     int i = _read( n_io, & x,  1 );
                s[ n++ ] = x;
			}
			s[ n ] = char( 0 );
			s_Topics.Add( s );

			if( eof( n_io) )
				{	
				::MessageBox
					(
					NULL, 
					"Ungerade Anzahl von Zeilen in DATA.TXT. Bitte die Datei überprüfen.", 
					"User Warnung", 
					 MB_OK | MB_ICONEXCLAMATION 
					 );
				break;
				}

			x = char( 0 );
			n = 0;
			while ( x != '\n'  )
			{
			     int i = _read( n_io, & x,  1 );
                s[ n++ ] = x;
			}
			s[ n ] = char( 0 );
           s_Explanations.Add( s );
		   b_Alive.Add(  TRUE );
		}
		_close( n_io);
		SetDlgItemTextA( IDC_TOPIC,  "Ready!" );	