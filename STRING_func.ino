
// FUnction which changes a tag with a string
bool replaceTagWithString(char* resultIn, size_t ResultBufferSize, char* searchString, char* tag, char * replaceWith)
{
  char * pch;
  pch = strstr (searchString, tag);

  if (ResultBufferSize - strlen(searchString) + strlen(tag) - strlen(replaceWith) < 0)
  {
    return false;
  }


  if (pch)
  {
    int temp = pch - searchString;
    strncpy ( resultIn, searchString, temp );
    resultIn[temp] = '\0';
    strcat(resultIn, replaceWith);
  
    strcat(resultIn, searchString + strlen(tag) + temp );
    
    return true;
  }


  return false;
}
