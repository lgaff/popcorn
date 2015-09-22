require 'rake'
require 'yaml'

ARCH = "x86_64"
TARGET = "#{ARCH}-unknown-none-elf"
CC = "clang"
CFLAGS = "-ffreestanding --sysroot=sysroot -target #{TARGET} -c"
AS = "nasm"
ASFLAGS = "-f elf64"
LDFLAGS = "-m elf_x86_64 -gc-sections -static -nostartfiles -nodefaultlibs"

PROJECTS = ["popcorn", "libc"]

SOURCE_FILES = Rake::FileList.new("src/**/*.s", "src/**/*.S", "src/**/*.c").exclude(/crt..S/)

CRTI_OBJ="obj/popcorn/arch/#{ARCH}/crti.o"
CRTBEGIN_OBJ=%x[#{CC} -print-file-name=crtbegin.o].chomp
CRTEND_OBJ=%x[#{CC} -print-file-name=crtend.o].chomp
CRTN_OBJ="obj/popcorn/arch/#{ARCH}/crtn.o"

task :default => [:sysroot, :popcorn]

task :popcorn => ["src/buildver.inc", :objects, CRTI_OBJ, CRTN_OBJ] do
  sh "ld #{LDFLAGS} -T src/popcorn/arch/#{ARCH}/link.ld -o sysroot/boot/popcorn #{CRTI_OBJ} #{CRTBEGIN_OBJ} #{SOURCE_FILES.pathmap("%{^src,obj}X.o")} #{CRTEND_OBJ} #{CRTN_OBJ}" #"
end


task :objects => SOURCE_FILES.pathmap("%{^src,obj}X.o")

file "src/buildver.inc" do
  version = YAML.load_file('version.yml')
  sh "echo 'VERSION db \"Popcorn kernel v#{version['major']}.#{version['minor']}.#{version['patch']}-#{version['additional_labels']}\", 0' > src/buildver.inc"
  sh "echo 'BUILD_DATE db \"Build date #{DateTime.now}\", 0' >> src/buildver.inc"
  # SOURCE_FILES.include("src/buildver.inc")
end


directory "obj"

rule ".o" => [->(f){f.pathmap("%{^obj,src}X.S")}, "obj"] do |srcfile|
  mkdir_p srcfile.name.pathmap("%d")
  sh "#{AS} #{ASFLAGS} -o #{srcfile.name} #{srcfile.source}"
end

rule ".o" => [->(f){f.pathmap("%{^obj,src}X.c")}, "obj"] do |srcfile|
  mkdir_p srcfile.name.pathmap("%d")
  sh "#{CC} #{CFLAGS} -o #{srcfile.name} #{srcfile.source}"
end

#todo write rule to make rnux.dsk image
directory "img"
task :qemu_disk => ["popcorn", :sysroot, "rnux.dsk", "img"] do
  loopfile = `losetup -f`.chomp
  if loopfile == ''
    raise "No free loopback devices available"
  else
    sh "sudo losetup #{loopfile} -o 1048576 rnux.dsk"
    sh "sudo mount #{loopfile} img"
    sh "sudo cp -rv sysroot/* img/"
    sh "sudo cp popcorn img/boot/"
    sh "sudo umount img"
    sh "sudo losetup -d #{loopfile}"
  end
end

task :iso => :popcorn do
  sh "grub-mkrescue -o popcorn.iso sysroot"
end

task :sysroot do
  sh "mkdir -p sysroot/usr"
  sh "mkdir -p sysroot/boot/grub/"
  sh "cp util/grub.cfg sysroot/boot/grub/"
  PROJECTS.each do |project|
    sh "cp -rv src/#{project}/include sysroot/usr/"
  end
end

task :clean do
  sh "rm -rf obj popcorn sysroot"
end
