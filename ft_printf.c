#include <stdarg.h>
#include <unistd.h>

typedef struct	s_flags {
	int		i;
	int		printed;
	int		width;
	int		precision;
	int		zeros;
	va_list	args;
}				t_flags;

void	init_flags(t_flags *flags)
{
	flags->i = 0;
	flags->printed = 0;
	flags->width = 0;
	flags->precision = 0;
	flags->zeros = 0;
}

void	reset_flags(t_flags *flags)
{
	flags->width = 0;
	flags->precision = 0;
	flags->zeros = 0;
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	print_till(t_flags *flags, const char *format, char c)
{
	while (format[flags->i] && format[flags->i] != c)
	{
		ft_putchar(format[flags->i]);
		flags->i++;
		flags->printed++;
	}
}

int		is_num(char c)
{
	return (c >= '0' && c <= '9');
}

void	handle_flags(t_flags *flags, const char *format)
{
	flags->i++;
	if (is_num(format[flags->i]))
	{
		while (is_num(format[flags->i]))
		{
			flags->width = flags->width * 10 + format[flags->i] - '0';
			flags->i++;
		}
	}
	if (format[flags->i] == '.')
	{
		flags->i++;
		flags->precision = 0;
		while (is_num(format[flags->i]))
		{
			flags->precision = flags->precision * 10 + format[flags->i] - '0';
			flags->i++;
		}
	}
}

int		ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	print_chars(int len, char c, t_flags *flags)
{
	int	i;

	i = -1;
	while (++i < len)
	{
		flags->printed++;
		ft_putchar(c);
	}
}

int		len_nbr_base(long nbr, int base_len)
{
	int len;

	len = 1;
	while (nbr >= base_len)
	{
		nbr /= base_len;
		len++;
	}
	return (len);
}

void	putnbr_base(long nbr, char *base, int base_len)
{
	if (nbr >= base_len)
	{
		putnbr_base(nbr / base_len, base, base_len);
		putnbr_base(nbr % base_len, base, base_len);
	}
	else
		ft_putchar(base[nbr]);
}

void	print_s(t_flags *flags, const char *format)
{
	char	*s;
	int		len;

	flags->i++;
	s = va_arg(flags->args, char *);
	if (!s)
		s = "(null)";
	len = ft_strlen(s);
	if (flags->precision && flags->precision < len)
		len = flags->precision;
	print_chars(flags->width - len, ' ', flags);
	write(1, s, len);
	flags->printed += len;
}

void	print_d(t_flags *flags, const char *format)
{
	long	nbr;
	int		len;
	int		neg;

	flags->i++;
	nbr = va_arg(flags->args, int);
	neg = 0;
	if (nbr < 0)
	{
		neg = 1;
		nbr = -nbr;
	}
	len = len_nbr_base(nbr, 10);
	if (nbr == 0 && flags->precision)
		len = 0;
	if (flags->precision && flags->precision > len)
		flags->zeros = flags->precision - len;
	if (neg)
		len++;
	print_chars(flags->width - (flags->zeros + len), ' ', flags);
	if (neg)
		ft_putchar('-');
	print_chars(flags->zeros, '0', flags);
	if (flags->precision && nbr == 0)
		return ;
	putnbr_base(nbr, "0123456789", 10);
	flags->printed += len;
}

void	print_x(t_flags *flags, const char *format)
{
	unsigned int	nbr;
	int				len;	

	flags->i++;
	nbr = va_arg(flags->args, unsigned int);
	len = len_nbr_base(nbr, 16);
	if (nbr == 0 && flags->precision)
		len = 0;
	if (flags->precision && flags->precision > len)
		flags->zeros = flags->precision - len;
	print_chars(flags->width - (flags->zeros + len), ' ', flags);
	print_chars(flags->zeros, '0', flags);
	if (flags->precision && nbr == 0)
		return ;
	putnbr_base(nbr, "0123456789abcedf", 16);
	flags->printed += len;
}

int		ft_printf(const char *format, ...)
{
	t_flags flags;

	init_flags(&flags);
	va_start(flags.args, format);
	while (format[flags.i])
	{
		reset_flags(&flags);
		print_till(&flags, format, '%');
		if (format[flags.i])
		{
			handle_flags(&flags, format);
			if (format[flags.i] == 's')
				print_s(&flags, format);
			if (format[flags.i] == 'd')
				print_d(&flags, format);
			if (format[flags.i] == 'x')
				print_x(&flags, format);
		}
	}
	return (flags.printed);
}

int main()
{
	ft_printf("%d %x %s %6.8d\n", 105, 44, "bonjour !", 8935);
	printf("%d %x %s %6.8d", 105, 44, "bonjour !", 8935);
	return (0);
}
