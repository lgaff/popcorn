require 'rake'
require 'yaml'

## Various incarnations of architecture selection
ARCH='x86_64'
TARGET="#{ARCH}-unknown-none-elf"
LDTARGET="elf_x86_64"
ASTARGET="elf64"

## Toolchain
CC = "clang"
LD = "ld"
AS = "nasm"
AR = "ar"

## Build folders
SYSROOT="sysroot"
OBJDIR="obj"
BOOT="#{SYSROOT}/boot/grub"

directory OBJDIR
PROJECTS= ["popcorn", "libc"]
## System root folders
directory "#{SYSROOT}/boot/grub"
directory "#{SYSROOT}/usr/include"
directory "#{SYSROOT}/usr/lib"

task :default => %w[popcorn:build]

task :rootdirs => [ "#{SYSROOT}/boot/grub", "#{SYSROOT}/usr/include", "#{SYSROOT}/usr/lib" ]

task :system => [ :rootdirs ] do
  sh "cp -v util/grub.cfg #{SYSROOT}/boot/grub/"
  PROJECTS.each do |project|
    sh "cp -rv #{project}/include #{SYSROOT}/usr/"
  end
  sh "cp -rv popcorn/arch/#{ARCH}/include #{SYSROOT}/usr/" if Dir.exists?( "popcorn/arch/#{ARCH}/include" )
  sh "cp -rv include #{SYSROOT}/usr/"  
end

file "#{SYSROOT}/usr/lib/libk.a" => "libk:build"
file "#{SYSROOT}/usr/lib/libc.a" => "libc:build"

namespace :popcorn do
  POPCORN_CFLAGS = "-ffreestanding --sysroot=#{SYSROOT} -target #{TARGET} -Wall -Wextra -mcmodel=kernel -mno-sse -mno-mmx"
  POPCORN_CPPFLAGS = "-D_is_popcorn_kernel"
  POPCORN_ASFLAGS = "-f #{ASTARGET}"
  POPCORN_LDFLAGS = "-static -L#{SYSROOT}/usr/lib -m #{LDTARGET} -nostdlib -nostartfiles -nodefaultlibs"
  POPCORN_LIBS = "-lk"
  
  # Sources, generic and platform specific
  COMMON_SOURCES=Rake::FileList.new("popcorn/**/*.S", "popcorn/**/*.cpp")
           .exclude(/arch/)
  ARCH_ASM_SOURCES=Rake::FileList.new("popcorn/arch/#{ARCH}/**/*.S").exclude(/crt..S/)
  ARCH_CPP_SOURCES=Rake::FileList.new("popcorn/arch/#{ARCH}/**/*.cpp")

  CRTI_OBJ="#{OBJDIR}/popcorn/arch/#{ARCH}/crti.o"
  CRTBEGIN_OBJ=%x[#{CC} -print-file-name=crtbegin.o].chomp
  CRTEND_OBJ=%x[#{CC} -print-file-name=crtend.o].chomp
  CRTN_OBJ="#{OBJDIR}/popcorn/arch/#{ARCH}/crtn.o"

  task :build => [ :system, OBJDIR, "buildver.inc", :objects, CRTI_OBJ, CRTN_OBJ, "#{SYSROOT}/usr/lib/libk.a"] do
    arch_asm_objects = ARCH_ASM_SOURCES.pathmap("%{^,#{OBJDIR}/}X.o")
    arch_cpp_objects = ARCH_CPP_SOURCES.pathmap("%{^,#{OBJDIR}/}X.o")
    common_objects = COMMON_SOURCES.pathmap("%{^,#{OBJDIR}/}X.o")
    objects = "#{CRTI_OBJ} #{CRTBEGIN_OBJ} #{arch_asm_objects} #{arch_cpp_objects} #{common_objects} #{CRTEND_OBJ} #{CRTN_OBJ}"
    sh "#{LD} #{POPCORN_LDFLAGS} -T popcorn/arch/#{ARCH}/link.ld -o #{SYSROOT}/boot/popcorn #{objects} #{POPCORN_LIBS}"
  end

  task :objects => [:common, :arch_asm, :arch_cpp]
  task :common => COMMON_SOURCES.pathmap("%{^,#{OBJDIR}/}X.o")
  task :arch_asm => ARCH_ASM_SOURCES.pathmap("%{^,#{OBJDIR}/}X.o")
  task :arch_cpp => ARCH_CPP_SOURCES.pathmap("%{^,#{OBJDIR}/}X.o")  

  file "buildver.inc" do
    version = YAML.load_file('version.yml')
    sh "echo 'VERSION db \"Popcorn kernel v#{version['major']}.#{version['minor']}.#{version['patch']}-#{version['additional_labels']}\", 0' > buildver.inc"
    sh "echo 'BUILD_DATE db \"Build date #{DateTime.now}\", 0' >> buildver.inc"
  end

  rule ".o" => [->(f){f.pathmap("%{^#{OBJDIR}/,}X.S")}, OBJDIR] do |srcfile|
    mkdir_p srcfile.name.pathmap("%d")
    sh "#{AS} #{POPCORN_ASFLAGS} -o #{srcfile.name} #{srcfile.source}"
  end

  rule ".o" => [->(f){f.pathmap("%{^#{OBJDIR}/,}X.c")}, OBJDIR] do |srcfile|
    mkdir_p srcfile.name.pathmap("%d")
    sh "#{CC} #{POPCORN_CFLAGS} #{POPCORN_CPPFLAGS} -o #{srcfile.name} -c #{srcfile.source}"
  end

  rule ".o" => [->(f){f.pathmap("%{^#{OBJDIR}/,}X.cpp")}, OBJDIR] do |srcfile|
    mkdir_p srcfile.name.pathmap("%d")
    sh "#{CC} #{POPCORN_CFLAGS} #{POPCORN_CPPFLAGS} -std=c++11 -o #{srcfile.name} -c #{srcfile.source}"
  end

end

namespace :libk do

  LIBK_CFLAGS = "-ffreestanding --sysroot=#{SYSROOT} -target #{TARGET} -Wall -Wextra -mcmodel=kernel -mno-sse -mno-mmx"
  LIBK_CPPFLAGS = "-D__is_popcorn_libc -D__is_popcorn_kernel"
  LIBK_ASFLAGS = "-f #{ASTARGET}"
  LIBK_LDFLAGS = "-m #{LDTARGET} -nostdlib -static -nostartfiles -nodefaultlibs"

  LIBK_SOURCES= Rake::FileList.new("libc/**/*.cpp", "libc/**/*.c")
  
  task :build => [:system, OBJDIR, :objects] do
    objects = LIBK_SOURCES.pathmap("%{^,#{OBJDIR}/}X.libk.o")
    sh "#{AR} rcs #{SYSROOT}/usr/lib/libk.a #{objects}"
  end

  task :objects => LIBK_SOURCES.pathmap("%{^,#{OBJDIR}/}X.libk.o")

  rule ".libk.o" => [->(f){f.pathmap("%{^#{OBJDIR}/,}d/%{.libk.o,}f.c")}, OBJDIR] do |srcfile|
    mkdir_p srcfile.name.pathmap("%d")
    sh "#{CC} #{LIBK_CFLAGS} #{LIBK_CPPFLAGS} -o #{srcfile.name} -c #{srcfile.source}"
  end

  rule ".libk.o" => [->(f){f.pathmap("%{^#{OBJDIR}/,}d/%{.libk.o,}f.cpp")}, OBJDIR] do |srcfile|
    mkdir_p srcfile.name.pathmap("%d")
    sh "#{CC} #{LIBK_CFLAGS} #{LIBK_CPPFLAGS} -std=c++11 -o #{srcfile.name} -c #{srcfile.source}"
  end
    
end

namespace :libc do
  LIBC_CFLAGS = "-ffreestanding --sysroot=#{SYSROOT} -target #{TARGET} -Wall -Wextra -mcmodel=kernel"
  LIBC_CPPFLAGS = "-D__is_popcorn_libc"
  LIBC_ASFLAGS = "-f #{ASTARGET}"
  LIBC_LDFLAGS = "-m #{LDTARGET} -nostdlib -static -nostartfiles -nodefaultlibs -std=gnu11"

  LIBC_SOURCES= Rake::FileList.new("libc/**/*.cpp", "libc/**/*.c")
  
  task :build => [:system, OBJDIR, :objects] do
    objects = LIBC_SOURCES.pathmap("%{^,#{OBJDIR}/}X.o")
    sh "#{AR} rcs #{SYSROOT}/usr/lib/libc.a #{objects}"
  end

  task :objects => LIBC_SOURCES.pathmap("%{^,#{OBJDIR}/}X.o")

  rule ".o" => [->(f){f.pathmap("%{^#{OBJDIR}/,}X.c")}, OBJDIR] do |srcfile|
    mkdir_p srcfile.name.pathmap("%d")
    sh "#{CC} #{LIBC_CFLAGS} #{LIBC_CPPFLAGS} -o #{srcfile.name} -c #{srcfile.source}"
  end

  rule ".o" => [->(f){f.pathmap("%{^#{OBJDIR}/,}X.cpp")}, OBJDIR] do |srcfile|
    mkdir_p srcfile.name.pathmap("%d")
    sh "#{CC} #{LIBC_CFLAGS} #{LIBC_CPPFLAGS} -std=c++11 -o #{srcfile.name} -c #{srcfile.source}"
  end

    
end

task :iso => %w[popcorn:build] do
  sh "grub-mkrescue -o popcorn.iso #{SYSROOT}"
end

task :clean do
  sh "rm -rf #{SYSROOT}"
  sh "rm -rf #{OBJDIR}"
  sh "if [[ -e buildver.inc ]];then rm buildver.inc; fi"
end
