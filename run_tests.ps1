Write-Host "=== Hello world tests ==="
.\build\src\brainfuck.exe .\test\hello.bf --memory=30000
.\build\src\brainfuck.exe .\test\rdebath\Brainfuck\testing\Hello.b --memory=30000
.\build\src\brainfuck.exe .\test\rdebath\Brainfuck\testing\Hello2.b --memory=30000
Write-Host "=== Golden ratio ==="
.\build\src\brainfuck.exe .\test\rdebath\Brainfuck\testing\Golden.b --memory=30000
Write-Host ""
Write-Host "=== al-count-1.b ==="
.\build\src\brainfuck.exe .\test\rdebath\Brainfuck\testing\al-count-1.b --memory=30000