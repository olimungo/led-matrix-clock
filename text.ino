void scrollText(char* text) {
  int x = (NUM_DEVICES * 8) - 1;
  int len = strlen(text);

  lmd.clear();

  while(x >= len * -8) {
    drawString(text, len, x);
    lmd.display();
    delay(ANIM_DELAY);

    if (checkButton()) {
      break;
    }

    x--;
  }
}

void drawString(char* text, int len, int x)
{
  for( int idx = 0; idx < len; idx ++ )
  {
    int c = text[idx] - 32;

    // stop if char is outside visible area
    if( x + idx * 8  > (NUM_DEVICES * 8) - 1 )
      return;

    // only draw if char is visible
    if( 8 + x + idx * 8 > 0 )
      drawSprite(alphabet[c], x + idx * 8, 0, 8, 8 );
  }
}

void drawSprite( byte* sprite, int x, int y, int width, int height )
{
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;
  
  for( int iy = 0; iy < height; iy++ )
  {
    for( int ix = 0; ix < width; ix++ )
    {
      lmd.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask ));

      // shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // reset column mask
    mask = B10000000;
  }
}
