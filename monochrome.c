// zou misg werken
// file in visual basic

Public Function ToBlackAndWhite(ByVal bmp As Bitmap) As Bitmap
        Dim x As Integer
        Dim y As Integer
        Dim gem As Integer
        Dim r, g, b As Integer
        Dim col As Color

        For x = 0 To bmp.Width - 1
            For y = 0 To bmp.Height - 1

                col = bmp.GetPixel(x, y)
                r = col.R
                g = col.G
                b = col.B
                gem = (r + g + b) / 3

                If gem > 128 Then
                    bmp.SetPixel(x, y, Color.White)
                Else
                    bmp.SetPixel(x, y, Color.Black)
                End If

            Next y
        Next x

        ToBlackAndWhite = bmp
    End Function
